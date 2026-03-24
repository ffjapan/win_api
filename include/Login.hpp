#pragma once
#include "Screen.hpp"
#include "Button.hpp"

class Login : public Screen {
    public:
        Button btn_to_taskviewer;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
        void onResize(int w, int h) override;
};