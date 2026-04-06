#include "Login.h"
#include "Message.h"
#include <windows.h>

namespace {

enum LoginControlId {
    ID_COMBO_USER = 1001,
    ID_EDIT_PASSWORD = 1002,
    ID_BTN_LOGIN = 1003,
    ID_BTN_NEW_USER = 1004
};

}  // namespace

Login::~Login() {
    if (font_ != nullptr) {
        DeleteObject(font_);
        font_ = nullptr;
    }
}

void Login::SetupFont() {
    if (font_ != nullptr) {
        DeleteObject(font_);
    }

    font_ = CreateFontW(
        -16, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Meiryo UI"
    );

    if (font_ == nullptr) {
        return;
    }

    HWND controls[] = {
        lbl_title.Handle(),
        lbl_user.Handle(),
        lbl_password.Handle(),
        combo_user.Handle(),
        edit_password.Handle(),
        btn_login.Handle(),
        btn_register_user.Handle()
    };

    for (HWND h : controls) {
        if (h != nullptr) {
            SendMessageW(h, WM_SETFONT, (WPARAM)font_, TRUE);
        }
    }
}

LRESULT Login::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            lbl_title.Create(Handle(), L"ログイン", 260, 70, 280, 32);
            lbl_user.Create(Handle(), L"ユーザ名", 200, 140, 120, 24);
            lbl_password.Create(Handle(), L"パスワード", 200, 200, 120, 24);

            combo_user.Create(Handle(), 340, 140, 220, 220, ID_COMBO_USER);
            combo_user.AddItem(L"佐藤");
            combo_user.AddItem(L"鈴木");
            combo_user.AddItem(L"田中");
            combo_user.SetSelectedIndex(0);

            edit_password.CreatePassword(Handle(), 340, 200, 220, 28, ID_EDIT_PASSWORD);

            btn_login.Create(Handle(), L"ログインする", 340, 260, ID_BTN_LOGIN);
            btn_register_user.Create(Handle(), L"新規登録", 620, 520, ID_BTN_NEW_USER);
            SetupFont();
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_LOGIN) {
                SendMessageW(GetParent(Handle()), MSG_GOTO_TASKVIEWER, 0, 0);
                return 0;
            }
            if (LOWORD(wp) == ID_BTN_NEW_USER) {
                MessageBoxW(Handle(), L"新規登録機能は次ステップで実装予定です。", L"確認", MB_OK);
                return 0;
            }
            return 0;
        case WM_DESTROY:
            if (font_ != nullptr) {
                DeleteObject(font_);
                font_ = nullptr;
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}

void Login::onResize(int w, int h) {
    const int center_x = w / 2;
    const int form_left = center_x - 220;
    const int input_left = form_left + 140;

    MoveWindow(lbl_title.Handle(), center_x - 140, 70, 280, 32, TRUE);
    MoveWindow(lbl_user.Handle(), form_left, 140, 120, 24, TRUE);
    MoveWindow(combo_user.Handle(), input_left, 140, 220, 220, TRUE);
    MoveWindow(lbl_password.Handle(), form_left, 200, 120, 24, TRUE);
    MoveWindow(edit_password.Handle(), input_left, 200, 220, 28, TRUE);
    MoveWindow(btn_login.Handle(), input_left, 260, 120, 30, TRUE);

    // 新規登録ボタンはログイン導線から離して右下に配置
    MoveWindow(btn_register_user.Handle(), w - 150, h - 60, 120, 30, TRUE);
}