#include "PullDown.h"

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

void PullDown::ClearItems() {
    SendMessageW(hwnd_, CB_RESETCONTENT, 0, 0);
}

void PullDown::SetSelectedIndex(int index) {
    SendMessageW(hwnd_, CB_SETCURSEL, index, 0);
}

int PullDown::GetSelectedIndex() const {
    return static_cast<int>(SendMessageW(hwnd_, CB_GETCURSEL, 0, 0));
}

std::wstring PullDown::GetTextByIndex(int index) const {
    if (index < 0) {
        return L"";
    }

    LRESULT len = SendMessageW(hwnd_, CB_GETLBTEXTLEN, index, 0);
    if (len == CB_ERR) {
        return L"";
    }

    std::wstring buf(static_cast<size_t>(len) + 1, L'\0');
    SendMessageW(hwnd_, CB_GETLBTEXT, index, (LPARAM)buf.data());
    buf.resize(static_cast<size_t>(len));
    return buf;
}

std::wstring PullDown::GetSelectedText() const {
    return GetTextByIndex(GetSelectedIndex());
}
