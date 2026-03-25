#ifndef APP_TASKVIEWER_H
#define APP_TASKVIEWER_H

#include "MyWinAPI/Screen.h"
#include "MyWinAPI/Button.h"

class TaskViewer : public Screen {
    public:
        Button btn_to_login;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
        void onResize(int w, int h) override;
};

#endif  // APP_TASKVIEWER_H
