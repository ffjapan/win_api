#ifndef APP_MESSAGE_H
#define APP_MESSAGE_H

#include <windows.h>

// 明示的に+nを書かなきゃいつか壊れるらしい
enum AppMessage {
    MSG_GOTO_LOGIN = WM_APP + 1,
    MSG_GOTO_TASKVIEWER = WM_APP + 2
};

#endif  // APP_MESSAGE_H
