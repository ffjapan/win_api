#include "Button.h"

bool Button::Create(HWND parent, LPCWSTR text, int x, int y, int id) {
    return CreateControl(
        L"BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0,
        x, y, 120, 30,
        parent,
        (HMENU)(UINT_PTR)id
    );
}
