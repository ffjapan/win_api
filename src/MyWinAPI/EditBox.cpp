#include "EditBox.h"

bool EditBox::Create(HWND parent, int x, int y, int w, int h, int id) {
    return CreateControl(
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |
        ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
        0,
        x, y, w, h,
        parent,
        (HMENU)(UINT_PTR)id
    );
}

std::wstring EditBox::GetText() {
    int len = GetWindowTextLengthW(hwnd_);
    std::wstring buf(len, L'\0');
    GetWindowTextW(hwnd_, buf.data(), len + 1);
    return buf;
}
