#include "Window.hpp"
#include "EditBox.hpp"

class MyWindow : public Window {
protected:
    LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override {
        switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(Handle(), &ps);
            TextOutW(hdc, 10, 10, L"Hello Win32", 11);
            EndPaint(Handle(), &ps);
            return 0;
        }
        }
        return Window::HandleMessage(msg, wp, lp);
    }
};


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    EditBox editbox{};
    // WNDCLASS 登録
    WNDCLASSW wc = {};
    wc.lpfnWndProc = MyWindow::WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyAppClass";
    RegisterClassW(&wc);

    // ウィンドウ作成
    MyWindow win;
    win.Create(L"MyAppClass", L"Hello", WS_OVERLAPPEDWINDOW);
    
    editbox.Create(win.Handle(), 10, 10, 100, 100, 10);
    ShowWindow(win.Handle(), SW_SHOW);

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;

}