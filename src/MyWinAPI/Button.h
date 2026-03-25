#ifndef MYWINAPI_BUTTON_H
#define MYWINAPI_BUTTON_H

#include "Control.h"

class Button : public Control {
    public:
        bool Create(HWND parent, LPCWSTR text, int x, int y, int id);
};

#endif  // MYWINAPI_BUTTON_H
