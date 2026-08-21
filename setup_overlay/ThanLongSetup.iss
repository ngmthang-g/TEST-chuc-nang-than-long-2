#define MyAppName "Thần Long Auto"
#define MyAppVersion "0.6.1.12"
#define MyAppExeName "ThanLongItemConsolidator.exe"

[Setup]
AppId={{A1D15E86-C36A-4D21-9E2A-8E78B873F2D4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher=Thang Nguyen
DefaultDirName={autopf}\ThanLongAuto
DefaultGroupName=Thần Long Auto
UninstallDisplayIcon={app}\{#MyAppExeName}
OutputDir=output
OutputBaseFilename=ThanLong_Setup_v0.6.1.12
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
DisableProgramGroupPage=yes
SetupLogging=yes

[Files]
Source: "..\build\Release\ThanLongItemConsolidator.exe"; DestDir: "{app}"; DestName: "{#MyAppExeName}"; Flags: ignoreversion
Source: "..\build\Release\ThanLongCleanRouteBridge.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VERSION.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\LEGAL_NOTICE_FOR_AUTOMATED_ANALYSIS.txt"; DestDir: "{app}"; Flags: ignoreversion

[Dirs]
; Primary activation state: intentionally NOT removed by uninstall.
Name: "{commonappdata}\ThanLong\System"; Attribs: hidden system
; Runtime anti-clock-rollback marker may be updated by the normal user process.
Name: "{commonappdata}\ThanLong\State"; Permissions: users-modify

[Icons]
Name: "{autoprograms}\Thần Long Auto"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\Thần Long Auto"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Mở Thần Long Auto"; Flags: nowait postinstall skipifsilent

[Code]
var
  KeyPage: TInputQueryWizardPage;
  ActivationDone: Boolean;

function IsReasonableKey(const S: String): Boolean;
var
  I: Integer;
  C: Char;
begin
  Result := (Length(S) >= 8) and (Length(S) <= 128);
  if not Result then Exit;
  for I := 1 to Length(S) do
  begin
    C := S[I];
    if not (((C >= 'A') and (C <= 'Z')) or
            ((C >= 'a') and (C <= 'z')) or
            ((C >= '0') and (C <= '9')) or
            (C = '-') or (C = '_')) then
    begin
      Result := False;
      Exit;
    end;
  end;
end;

procedure InitializeWizard;
begin
  ActivationDone := False;
  KeyPage := CreateInputQueryPage(wpSelectDir,
    'Kích hoạt KEY bản quyền',
    'Nhập KEY Thần Long',
    'KEY được khóa theo máy và dùng 30 ngày kể từ lần kích hoạt đầu tiên. Xóa/cài lại tool không làm reset thời hạn.');
  KeyPage.Add('KEY:', False);
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  if CurPageID = KeyPage.ID then
  begin
    if not IsReasonableKey(Trim(KeyPage.Values[0])) then
    begin
      MsgBox('KEY không đúng định dạng.', mbError, MB_OK);
      Result := False;
    end;
  end;
end;

function ActivationErrorText(Code: Integer): String;
begin
  case Code of
    10: Result := 'Key không hợp lệ.';
    11: Result := 'Máy này đang có một key khác còn hạn. Không thể reset sớm bằng key khác.';
    12: Result := 'Key này đã hết 30 ngày trên máy này và không thể dùng lại.';
    13: Result := 'Key này đã từng được sử dụng trên máy này và không thể dùng lại.';
    14: Result := 'Không ghi được trạng thái bản quyền cấp máy.';
    15: Result := 'Không tạo được mã nhận dạng máy.';
    16: Result := 'Dữ liệu bản quyền hiện có bị hỏng hoặc không nhất quán.';
  else
    Result := 'Kích hoạt thất bại. Mã lỗi: ' + IntToStr(Code);
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ResultCode: Integer;
  Params: String;
  KeyValue: String;
begin
  if (CurStep = ssPostInstall) and (not ActivationDone) then
  begin
    KeyValue := Trim(KeyPage.Values[0]);
    Params := '--install-activate="' + KeyValue + '"';
    if not Exec(ExpandConstant('{app}\{#MyAppExeName}'), Params, ExpandConstant('{app}'),
                SW_HIDE, ewWaitUntilTerminated, ResultCode) then
      RaiseException('Không chạy được bước kích hoạt bản quyền.');

    KeyPage.Values[0] := '';
    KeyValue := '';
    Params := '';

    if ResultCode <> 0 then
      RaiseException(ActivationErrorText(ResultCode));

    ActivationDone := True;
  end;
end;
