#pragma once
#include <windows.h>

class Control {
    public:
        Control() : hwnd_(nullptr) {};
        HWND Handle() const;

    protected:
        HWND hwnd_;

        HWND CreateControl(
            LPCWSTR class_name,
            LPCWSTR text,
            DWORD style,
            DWORD ex_style,
            int x, int y, int w, int h,
            HWND parent,
            HMENU id
        );
};