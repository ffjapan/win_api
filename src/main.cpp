#include <windows.h>
#include "Window.hpp"
#include "Login.hpp"
#include "TaskViewer.hpp"
#include "Message.hpp"

class MyWindow : public Window {
    public:
        // 作る画面を宣言する
        Login screen_login;
        TaskViewer screen_taskviewer;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override {
            switch (msg) {
                case WM_CREATE:  // ウィンドウが作られたとき
                    screen_login.Create(
                        L"MyAppClass",  // ここでメインの識別名を指定している
                        L"",
                        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                        0,
                        0, 0, 800, 800,  // x, y, w, h
                        Handle(),
                        nullptr
                    );

                    screen_taskviewer.Create(
                        L"MyAppClass",
                        L"",
                        WS_CHILD| WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                        0,
                        0, 0, 800, 800,
                        Handle(),
                        nullptr
                    );

                    screen_login.Show();
                    screen_taskviewer.Hide();
                    return 0;

                case MSG_GOTO_LOGIN:
                    screen_taskviewer.Hide();
                    screen_login.Show();
                    return 0;
                
                case MSG_GOTO_TASKVIEWER:
                    screen_login.Hide();
                    screen_taskviewer.Show();
                    return 0;
                
                case WM_SIZE:
                    int w = LOWORD(lp);
                    int h = HIWORD(lp);

                    MoveWindow(screen_login.Handle(), 0, 0, w, h, TRUE);
                    MoveWindow(screen_taskviewer.Handle(), 0, 0, w, h, TRUE);
                    return 0;
                    
            }
            return Window::HandleMessage(msg, wp, lp);
        }
};


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    /* ------------------------------------------------------------------------*/
    WNDCLASSW wc = {};
    wc.lpfnWndProc = MyWindow::WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyAppClass";
    RegisterClassW(&wc);
    /* ----------------------ここまでおまじないみたいな感じ-----------------------*/

    // ウィンドウ作成
    MyWindow win;  // ここですべての画面構成が出来上がる
    win.Create(L"MyAppClass", L"タスク管理システム", WS_OVERLAPPEDWINDOW);  // 第一引数が識別名。これをメインとして画面を作る
    ShowWindow(win.Handle(), SW_SHOW);

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;

}