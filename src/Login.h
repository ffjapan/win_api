#ifndef APP_LOGIN_H
#define APP_LOGIN_H

#include "MyWinAPI/Screen.h"
#include "MyWinAPI/Button.h"
#include "MyWinAPI/PullDown.h"
#include "MyWinAPI/EditBox.h"
#include "MyWinAPI/Label.h"

class Login : public Screen {
    public:
        Label lbl_title;
        Label lbl_user;
        Label lbl_password;
        PullDown combo_user;
        EditBox edit_password;
        Button btn_login;
        Button btn_register_user;

        ~Login();
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
        void onResize(int w, int h) override;

    private:
        HFONT font_ = nullptr;
        void SetupFont();
};

#endif  // APP_LOGIN_H
