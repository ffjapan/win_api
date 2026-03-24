#pragma once
#include "Screen.hpp"
#include "Button.hpp"

class TaskViewer : public Screen {
    public:
        Button btn_to_login;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
        void onResize(int w, int h) override;
};