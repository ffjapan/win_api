#include "Control.hpp"

HWND Control::Handle() const {
    return hwnd_;
}

HWND Control::CreateControl(
    LPCWSTR class_name,
    LPCWSTR text,
    DWORD style,
    DWORD ex_style,
    int x, int y, int w, int h,
    HWND parent,
    HMENU id
) {
    hwnd_ = CreateWindowExW(
        ex_style,
        class_name,
        text,
        style,
        x, y, w, h,
        parent,
        id,
        GetModuleHandleW(nullptr),
        nullptr
    );
    return hwnd_;
}