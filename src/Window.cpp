#include "Window.hpp"

Window::Window() : hwnd_(nullptr) {}

bool Window::Create(
    LPCWSTR className,
    LPCWSTR title,
    DWORD style,
    DWORD exStyle,
    int x,
    int y,
    int w,
    int h,
    HWND parent,
    HMENU menu
) {
    hwnd_ = CreateWindowExW(
        exStyle,
        className,
        title,
        style,
        x, y, w, h,
        parent,
        menu,
        GetModuleHandleW(nullptr),
        this
    );
    return hwnd_ != nullptr;
}

HWND Window::Handle() const { return hwnd_; }

LRESULT Window::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd_, msg, wp, lp);
}

LRESULT CALLBACK Window::WindowProc(
    HWND hwnd, UINT msg, WPARAM wp, LPARAM lp
) {
    Window* self = nullptr;

    if (msg == WM_NCCREATE) {
        auto cs = reinterpret_cast<CREATESTRUCTW*>(lp);
        self = reinterpret_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)self);
        self->hwnd_ = hwnd;
    } else {
        self = reinterpret_cast<Window*>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA)
        );
    }

    if (self) {
        return self->HandleMessage(msg, wp, lp);
    }

    return DefWindowProcW(hwnd, msg, wp, lp);
}