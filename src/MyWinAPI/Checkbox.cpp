#include "Checkbox.h"

bool Checkbox::Create(HWND parent, LPCWSTR text, int x, int y, int id) {
    return CreateControl(
        L"BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        0,
        x, y, 120, 20,
        parent,
        (HMENU)(UINT_PTR)id
    );
}

bool Checkbox::IsChecked() const {
    return SendMessage(hwnd_, BM_GETCHECK, 0, 0) == BST_CHECKED;
}
