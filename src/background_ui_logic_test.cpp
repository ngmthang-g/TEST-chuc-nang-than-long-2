#include "background_ui_logic.h"

#include <cassert>

using namespace background_ui_logic;

int main() {
    assert(Key(L"ĐẦU THAI") == L"dauthai");
    assert(Key(L"Bán vật phẩm nhanh") == L"banvatphamnhanh");

    Labels confirm{L"ButtonOK", L"Xác nhận", L"", L"Root/MessageBox", L""};
    Labels cancel{L"ButtonCancel", L"Hủy", L"", L"Root/MessageBox", L""};
    Labels unrelated{L"ButtonOK", L"Xác nhận", L"", L"ShopPanel", L""};
    assert(Score(confirm, Role::ConfirmMap) > 0);
    assert(Score(cancel, Role::ConfirmMap) < 0);
    assert(Score(unrelated, Role::ConfirmMap) == 0);

    Labels revive{L"BtnRevive", L"Đầu thai", L"", L"DeathPanel", L""};
    assert(Score(revive, Role::Revive) > 0);
    Labels nestedRevive{L"Button_12", L"", L"", L"DeathPanel", L"Label/Đầu thai"};
    assert(Score(nestedRevive, Role::Revive) > 0);

    Labels quickSell{L"QuickSell", L"Bán vật phẩm nhanh", L"", L"Shop/Bag", L""};
    assert(Score(quickSell, Role::QuickSell) > Score(quickSell, Role::SellTab));

    Labels bagItem{L"BagItemCell_04", L"", L"OnItemClick", L"Bag/Equipment/ItemGrid", L""};
    Labels shopItem{L"ShopItemCell_04", L"", L"OnItemClick", L"Shop/ProductItemList", L""};
    assert(SafeBagItem(bagItem));
    assert(!SafeBagItem(shopItem));

    Labels safeClose{L"BtnClose", L"", L"", L"Shop/BagPanel", L""};
    Labels unsafeClose{L"BtnClose", L"", L"", L"QuestPanel", L""};
    assert(Score(safeClose, Role::CloseTradeOrBag) > 0);
    assert(Score(unsafeClose, Role::CloseTradeOrBag) == 0);

    Labels autoRoot{L"BtnAuto", L"AUTO", L"", L"Root/TopIcon", L""};
    Labels autoTrain{L"BtnFightMonster", L"Đánh quái", L"", L"Root/TopIcon/AutoMenu", L""};
    Labels autoStop{L"BtnStopAuto", L"Dừng", L"", L"Root/TopIcon/AutoMenu", L""};
    Labels configTrainTab{L"TabMonster", L"Đánh quái", L"", L"AutoTrainMonster/Settings", L""};
    Labels unrelatedStop{L"BtnStop", L"Dừng", L"", L"QuestPanel", L""};
    assert(Score(autoRoot, Role::AutoRoot) > 0);
    assert(Score(autoRoot, Role::AutoTrain) == 0);
    assert(Score(autoTrain, Role::AutoTrain) > 0);
    assert(Score(configTrainTab, Role::AutoTrain) <= 0);
    assert(Score(autoStop, Role::AutoStop) > 0);
    assert(Score(unrelatedStop, Role::AutoStop) <= 0);

    Labels bagCell2{L"BagItemCell_02", L"", L"OnItemClick", L"Bag/Equipment/ItemGrid", L""};
    Labels bagCell10{L"BagItemCell_10", L"", L"OnItemClick", L"Bag/Equipment/ItemGrid", L""};
    Labels bagIcon{L"BagItemIcon_01", L"", L"", L"Bag/Equipment/ItemGrid", L""};
    assert(BagItemScore(bagCell2) > BagItemScore(bagIcon));
    assert(NaturalItemIndex(bagCell2) == 2);
    assert(NaturalItemIndex(bagCell10) == 10);
    return 0;
}
