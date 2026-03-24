#pragma once
#include "Screen.hpp"
#include "Button.hpp"

class Login : public Screen {
    public:
        Button btnToB;
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
};