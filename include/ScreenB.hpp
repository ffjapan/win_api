#pragma once
#include "Screen.hpp"
#include "Button.hpp"

class ScreenB : public Screen {
    public:
        Button btnToA;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
};