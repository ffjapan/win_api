#include "Control.hpp"
#include <string>

class PullDown : public Control {
    public:
        bool Create(HWND parent, int x, int y, int w, int h, int id);
        void AddItem(const std::wstring& text);
        int GetSelectedIndex() const;
};