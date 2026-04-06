#ifndef MYWINAPI_LISTVIEW_H
#define MYWINAPI_LISTVIEW_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "Control.h"

class ListView : public Control {
    public:
        static bool InitCommonControls();

        bool Create(HWND parent, int x, int y, int w, int h, int id = 0, DWORD style = 0);
        void SetExtendedStyle(DWORD style);
        int InsertColumn(int index, const std::wstring& header, int width);
        int InsertItem(int row, const std::wstring& text);
        bool SetItemText(int row, int col, const std::wstring& text);
        void DeleteAllItems();
        HWND HeaderHandle() const;
};

#endif  // MYWINAPI_LISTVIEW_H
