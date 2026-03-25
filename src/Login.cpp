#include "Login.h"
#include "Message.h"
#include <windows.h>

LRESULT Login::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            btn_to_taskviewer.Create(Handle(), L"タスク一覧へ", 20, 20, 101);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                SendMessageW(GetParent(Handle()), MSG_GOTO_TASKVIEWER, 0, 0);
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}

void Login::onResize(int w, int h) {
    MoveWindow(btn_to_taskviewer.Handle(), 100, 100, 120, 30, TRUE);
}