#ifndef APP_LOGIN_H
#define APP_LOGIN_H

#include "MyWinAPI/Screen.h"
#include "MyWinAPI/Button.h"

class Login : public Screen {
    public:
        Button btn_to_taskviewer;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
        void onResize(int w, int h) override;
};

#endif  // APP_LOGIN_H
