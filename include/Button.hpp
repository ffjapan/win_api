#pragma once
#include "Control.hpp"

class Button : public Control {
    public:
        bool Create(HWND parent, LPCWSTR text, int x, int y, int id);
};