namespace {

// v1.1.13 keeps the runtime-proven MoonSharp invocation from V122 and changes only
// the read-only GameDialog observer. V122 used rawget() for semantic fields, which
// intentionally bypasses __index/metatable lookup. The live V122 run proved the
// chunk executes but found no Selections through that raw-only representation.
static const wchar_t kGameDialogProbeLuaV123[] = LR"LUA(
local function cut(s, n)
    s = tostring(s or "")
    if #s <= n then return s end
    return string.sub(s, 1, n - 3) .. "..."
end
local function norm(s)
    if type(s) ~= "string" then return "" end
    s = string.gsub(s, "<[^>]*>", " ")
    s = string.gsub(s, "%s+", " ")
    s = string.gsub(s, "^%s+", "")
    s = string.gsub(s, "%s+$", "")
    return string.lower(s)
end
local function semanticTag(s)
    local n = norm(s)
    if n == "trị liệu" or string.find(n, "trị liệu", 1, true) == 1 then return "T" end
    if n == "xác nhận" or string.find(n, "xác nhận", 1, true) == 1 then return "C" end
    if n == "ta biết rồi" or string.find(n, "ta biết rồi", 1, true) == 1 then return "K" end
    return nil
end
local function safeget(t, key)
    if type(t) ~= "table" then return nil, false end
    local ok, value = pcall(function() return t[key] end)
    if not ok then return nil, false end
    return value, true
end

local found = { T=0, C=0, K=0 }
local where = { T="", C="", K="" }
local samples = {}
local keySamples = {}
local seen = {}
local nodes = 0
local selectionTables = 0
local selectionValues = 0
local metaTables = 0

local function addSample(s)
    if #samples < 4 then samples[#samples + 1] = cut(s, 72) end
end
local function addKey(path, key, value)
    if #keySamples < 7 and value ~= nil then
        keySamples[#keySamples + 1] = cut(path .. "." .. tostring(key) .. ":" .. type(value), 56)
    end
end
local function acceptText(path, key, text)
    if type(text) ~= "string" then return end
    addSample(path .. "[" .. tostring(key) .. "]=" .. text)
    local tag = semanticTag(text)
    local id = tonumber(key)
    if tag and id and id > 0 and found[tag] == 0 then
        found[tag] = id
        where[tag] = path
    end
end
local function scanSelections(sels, path)
    if type(sels) ~= "table" then return end
    selectionTables = selectionTables + 1
    for k, v in pairs(sels) do
        selectionValues = selectionValues + 1
        if type(v) == "string" then
            acceptText(path, k, v)
        elseif type(v) == "table" then
            local text = nil
            local textKeys = {"SelectionName", "selectionName", "Text", "text", "Name", "name", "Value", "value"}
            for _, tk in ipairs(textKeys) do
                local tv = safeget(v, tk)
                if type(tv) == "string" and tv ~= "" then text = tv; break end
            end
            if text then acceptText(path, k, text)
            else addSample(path .. "[" .. tostring(k) .. "]=<table>") end
        else
            addSample(path .. "[" .. tostring(k) .. "]=<" .. type(v) .. ">")
        end
    end
end

local scan
scan = function(t, path, depth)
    if type(t) ~= "table" or seen[t] or depth > 5 or nodes >= 160 then return end
    seen[t] = true
    nodes = nodes + 1

    -- Normal indexing is deliberate: unlike V122 rawget(), it can follow __index.
    local sels = safeget(t, "Selections")
    if sels ~= nil then
        addKey(path, "Selections", sels)
        scanSelections(sels, path .. ".Selections")
    end

    local priority = {
        "GameDialogData", "gameDialogData", "CurrentGameDialogData", "currentGameDialogData",
        "DialogData", "dialogData", "GameDialog", "gameDialog"
    }
    for _, key in ipairs(priority) do
        local v = safeget(t, key)
        if v ~= nil then
            addKey(path, key, v)
            if type(v) == "table" then scan(v, path .. "." .. key, depth + 1) end
        end
    end

    for k, v in pairs(t) do
        addKey(path, k, v)
        if type(v) == "table" then
            local ks = string.lower(tostring(k))
            if string.find(ks, "dialog", 1, true) or string.find(ks, "selection", 1, true) then
                scan(v, path .. "." .. tostring(k), depth + 1)
            end
        end
    end

    if type(getmetatable) == "function" then
        local mt = getmetatable(t)
        if type(mt) == "table" then
            metaTables = metaTables + 1
            local idx = rawget(mt, "__index")
            if idx ~= nil then addKey(path .. "@mt", "__index", idx) end
            if type(idx) == "table" then scan(idx, path .. "@__index", depth + 1) end
        end
    end

    for k, v in pairs(t) do
        if type(v) == "table" then scan(v, path .. "." .. tostring(k), depth + 1) end
    end
end

local gd = nil
local af = nil
if GUI and GUI.FindUI then
    gd = GUI.FindUI("GameDialog")
    af = GUI.FindUI("AutoFight_Main")
end
scan(gd, "GD", 0)
scan(af, "AF", 0)

-- Canonical data name is GameDialogData. If a build exposes it globally, observe it too.
if type(_G) == "table" then
    local globalGD = safeget(_G, "GameDialogData")
    if type(globalGD) == "table" then scan(globalGD, "_G.GameDialogData", 0) end
    local currentGD = safeget(_G, "CurrentGameDialogData")
    if type(currentGD) == "table" then scan(currentGD, "_G.CurrentGameDialogData", 0) end
end

return "T="..tostring(found.T)..";C="..tostring(found.C)..";K="..tostring(found.K)..
       ";GD="..type(gd)..";AF="..type(af)..";NODE="..tostring(nodes)..
       ";ST="..tostring(selectionTables)..";SV="..tostring(selectionValues)..";MT="..tostring(metaTables)..
       ";WT="..where.T..";WC="..where.C..";WK="..where.K..
       ";KS="..table.concat(keySamples, ",")..";S="..table.concat(samples, " | ")
)LUA";

struct LuaDialogIdsV123 {
    std::int32_t treatment = 0;
    std::int32_t confirm = 0;
    std::int32_t know = 0;
    wchar_t raw[512]{};
    wchar_t resolver[192]{};
};

bool ReadLuaDialogIdsV123(LuaDialogIdsV123& ids, wchar_t* detail, std::size_t cap) {
    ids = {};
    wchar_t raw[640]{};
    if (!RunLuaChunkV122(kGameDialogProbeLuaV123, raw, _countof(raw),
                         ids.resolver, _countof(ids.resolver), detail, cap)) return false;
    (void)ParseLuaTokenIntV119(raw, L"T=", ids.treatment);
    (void)ParseLuaTokenIntV119(raw, L"C=", ids.confirm);
    (void)ParseLuaTokenIntV119(raw, L"K=", ids.know);
    SetText(ids.raw, _countof(ids.raw), raw);
    return true;
}

bool InspectHealDialog(cleanroute::Snapshot& s, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap)) return false;
    s = {};
    s.validMask |= ValidHealDialog;

    bool gameDialogPresent = false;
    bool messageBoxPresent = false;
    if (!LuaUiPresentV119(c, L"GameDialog", gameDialogPresent, detail, cap)) return false;
    if (!LuaUiPresentV119(c, L"MessageBox", messageBoxPresent, detail, cap)) return false;
    if (gameDialogPresent) s.healDialogMask |= HealDialogPresent;

    LuaDialogIdsV123 ids{};
    wchar_t probe[640]{};
    if (!ReadLuaDialogIdsV123(ids, probe, _countof(probe))) {
        SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V123 PROBE FAIL • ");
        Append(s.healDialogDetail, _countof(s.healDialogDetail), probe);
        SetText(detail, cap, s.healDialogDetail);
        return true;
    }

    if (ids.treatment > 0) s.healDialogMask |= HealDialogTreatment;
    if (ids.confirm > 0 || messageBoxPresent) s.healDialogMask |= HealDialogConfirm;
    if (ids.know > 0) s.healDialogMask |= HealDialogKnow;

    // Put semantic IDs and observer diagnostics before the route so truncation keeps useful evidence.
    SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V123 • T=");
    AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.treatment);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • C="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.confirm);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • K="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.know);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • GD=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), gameDialogPresent ? L"present" : L"absent");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • MB=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), messageBoxPresent ? L"present" : L"absent");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • raw={"); Append(s.healDialogDetail, _countof(s.healDialogDetail), ids.raw); Append(s.healDialogDetail, _countof(s.healDialogDetail), L"}");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • route=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), ids.resolver[0] ? ids.resolver : L"?");
    SetText(detail, cap, s.healDialogDetail);
    return true;
}

} // namespace
