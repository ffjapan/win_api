#pragma once
#include <windows.h>
#include <string>
#include "Control.hpp"

class EditBox : public Control {
    public:
        bool Create(HWND parent, int x, int y, int w, int h, int id);

        std::wstring GetText();
};