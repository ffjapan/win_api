#ifndef MYWINAPI_PULLDOWN_H
#define MYWINAPI_PULLDOWN_H

#include <string>
#include "Control.h"

class PullDown : public Control {
    public:
        bool Create(HWND parent, int x, int y, int w, int h, int id);
        void AddItem(const std::wstring& text);
        void ClearItems();
        void SetSelectedIndex(int index);
        int GetSelectedIndex() const;
        std::wstring GetTextByIndex(int index) const;
        std::wstring GetSelectedText() const;
};

#endif  // MYWINAPI_PULLDOWN_H
