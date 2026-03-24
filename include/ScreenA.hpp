#pragma once
#include "Screen.hpp"
#include "Button.hpp"

class ScreenA : public Screen {
    public:
        Button btnToB;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
};