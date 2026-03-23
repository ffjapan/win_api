#pragma once
#include <windows.h>
#include "Control.hpp"

class Checkbox : public Control {
    public:
        bool Create(HWND parent, LPCWSTR text, int x, int y, int id);
        bool IsChecked() const;
};