#ifndef MYWINAPI_LABEL_H
#define MYWINAPI_LABEL_H

#include "Control.h"

class Label : public Control {
    public:
        bool Create(HWND parent, LPCWSTR text, int x, int y, int w, int h, int id = 0);
};

#endif  // MYWINAPI_LABEL_H
