#ifndef MYWINAPI_CHECKBOX_H
#define MYWINAPI_CHECKBOX_H

#include <windows.h>
#include "Control.h"

class Checkbox : public Control {
    public:
        bool Create(HWND parent, LPCWSTR text, int x, int y, int id);
        bool IsChecked() const;
};

#endif  // MYWINAPI_CHECKBOX_H
