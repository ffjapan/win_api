#include "TaskViewer.hpp"
#include "Message.hpp"
#include <windows.h>

LRESULT TaskViewer::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            btn_to_login.Create(Handle(), L"ログイン画面へ", 20, 20, 101);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                SendMessageW(GetParent(Handle()), MSG_GOTO_LOGIN, 0, 0);
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}

void TaskViewer::onResize(int w, int h) {
    MoveWindow(btn_to_login.Handle(), 100, 100, 120, 30, TRUE);
}