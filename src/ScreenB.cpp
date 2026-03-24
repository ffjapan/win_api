#include "ScreenB.hpp"
#include <windows.h>

LRESULT ScreenB::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            btnToA.Create(Handle(), L"画面Aへ", 20, 20, 101);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                SendMessageW(GetParent(Handle()), WM_APP + 2, 0, 0);
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}