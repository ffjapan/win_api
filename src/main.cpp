#include <windows.h>
#include "Window.hpp"
#include "EditBox.hpp"
#include "Checkbox.hpp"
#include "PullDown.hpp"
#include "ScreenA.hpp"
#include "ScreenB.hpp"

class MyWindow : public Window {
    public:
        ScreenA screenA;
        ScreenB screenB;
protected:
    LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override {
        switch (msg) {
            case WM_CREATE:
                screenA.Create(
                    L"MyAppClass",
                    L"",
                    WS_CHILD | WS_VISIBLE,
                    0,
                    0, 0, 800, 800,
                    Handle(),
                    nullptr
                );

                screenB.Create(
                    L"MyAppClass",
                    L"",
                    WS_CHILD,
                    0,
                    0, 0, 800, 800,
                    Handle(),
                    nullptr
                );

                screenA.Show();
                screenB.Hide();
                return 0;

            case WM_APP + 1:
                screenA.Hide();
                screenB.Show();
                return 0;
            
            case WM_APP + 2:
                screenB.Hide();
                screenA.Show();
                return 0;
        }
        return Window::HandleMessage(msg, wp, lp);
    }
};


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    // EditBox editbox{};
    // Checkbox checkbox{};
    // PullDown pulldown{};
    // WNDCLASS 登録
    WNDCLASSW wc = {};
    wc.lpfnWndProc = MyWindow::WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyAppClass";
    RegisterClassW(&wc);

    // ウィンドウ作成
    MyWindow win;
    win.Create(L"MyAppClass", L"Hello", WS_OVERLAPPEDWINDOW);
    
    // editbox.Create(sc1.Handle(), 10, 10, 100, 100, 10);
    // checkbox.Create(sc1.Handle(), L"hello", 10, 150, 20);
    // pulldown.Create(sc1.Handle(), 200, 200, 200, 200, 30);
    // pulldown.AddItem(L"要件定義");
    // pulldown.AddItem(L"基本設計");

    ShowWindow(win.Handle(), SW_SHOW);

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;

}