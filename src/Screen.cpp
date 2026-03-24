#include "Screen.hpp"

void Screen::Show() {
    ShowWindow(Handle(), SW_SHOW);
}

void Screen::Hide() {
    ShowWindow(Handle(), SW_HIDE);
}

LRESULT Screen::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        // 画面サイズ変更時に空になる部分を白色で埋めている
        case WM_ERASEBKGND:
            RECT rc;
            GetClientRect(Handle(), &rc);
            FillRect((HDC)wp, &rc, (HBRUSH)(COLOR_WINDOW + 1));
            return 1;
        
        case WM_SIZE:
            onResize(LOWORD(lp), HIWORD(lp));
            return 0;
    }
    return Window::HandleMessage(msg, wp, lp);
}