#include "PullDown.hpp"

bool PullDown::Create(HWND parent, int x, int y, int w, int h, int id) {
    return CreateControl(
        L"COMBOBOX",
        L"",
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        0,
        x, y, w, h,
        parent,
        (HMENU)(UINT_PTR)id
    );
}

void PullDown::AddItem(const std::wstring& text) { 
    SendMessageW(hwnd_, CB_ADDSTRING, 0, (LPARAM)text.c_str());
}

int PullDown::GetSelectedIndex() const {
    return SendMessageW(hwnd_, CB_GETCURSEL, 0, 0);
}