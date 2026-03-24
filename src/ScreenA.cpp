#include "ScreenA.hpp"
#include <windows.h>

LRESULT ScreenA::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            btnToB.Create(Handle(), L"画面Bへ", 20, 20, 101);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                SendMessageW(GetParent(Handle()), WM_APP + 1, 0, 0);
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}