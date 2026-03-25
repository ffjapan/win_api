#ifndef MYWINAPI_SCREEN_H
#define MYWINAPI_SCREEN_H

#include "Window.h"

class Screen : public Window {
    public:
        virtual void Show();
        virtual void Hide();
    protected:
        virtual void onResize(int w, int h) {}
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
};

#endif  // MYWINAPI_SCREEN_H
