#pragma once
#include "Window.hpp"

class Screen : public Window {
    public:
        virtual void Show();
        virtual void Hide();
    protected:
        virtual void onResize(int w, int h){};
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
};