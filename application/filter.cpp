#define UNICODE
#define _UNICODE
#include <windows.h>
#include "filter.h"


void setComboItems(HWND hCombo, const std::vector<std::wstring>& items) {
    SendMessageW(hCombo, CB_RESETCONTENT, 0, 0);

    // 全選択用の空項目を最初に追加
    SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"ALL");

    //ほかの項目を追加
    for (auto& item : items) {
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
    }

    // デフォルトで「全選択」を選択
    SendMessageW(hCombo, CB_SETCURSEL, 0, 0);
}
