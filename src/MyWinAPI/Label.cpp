#include "Label.h"

bool Label::Create(HWND parent, LPCWSTR text, int x, int y, int w, int h, int id) {
    return CreateControl(
        L"STATIC",
        text,
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0,
        x, y, w, h,
        parent,
        (HMENU)(UINT_PTR)id
    );
}
