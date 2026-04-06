#ifndef MYWINAPI_EDITBOX_H
#define MYWINAPI_EDITBOX_H

#include <windows.h>
#include <string>
#include "Control.h"

class EditBox : public Control {
    public:
        bool Create(HWND parent, int x, int y, int w, int h, int id);
        bool CreatePassword(HWND parent, int x, int y, int w, int h, int id);

        std::wstring GetText();
};

#endif  // MYWINAPI_EDITBOX_H
