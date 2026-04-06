#include "ListView.h"

bool ListView::InitCommonControls() {
    INITCOMMONCONTROLSEX icex = {};
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    return InitCommonControlsEx(&icex) == TRUE;
}

bool ListView::Create(HWND parent, int x, int y, int w, int h, int id, DWORD style) {
    DWORD final_style = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | style;
    return CreateControl(
        WC_LISTVIEWW,
        L"",
        final_style,
        0,
        x, y, w, h,
        parent,
        (HMENU)(UINT_PTR)id
    );
}

void ListView::SetExtendedStyle(DWORD style) {
    ListView_SetExtendedListViewStyle(hwnd_, style);
}

int ListView::InsertColumn(int index, const std::wstring& header, int width) {
    LVCOLUMNW col = {};
    col.mask = LVCF_TEXT | LVCF_WIDTH;
    col.cx = width;
    col.pszText = const_cast<LPWSTR>(header.c_str());
    return ListView_InsertColumn(hwnd_, index, &col);
}

int ListView::InsertItem(int row, const std::wstring& text) {
    LVITEMW item = {};
    item.mask = LVIF_TEXT;
    item.iItem = row;
    item.pszText = const_cast<LPWSTR>(text.c_str());
    return ListView_InsertItem(hwnd_, &item);
}

bool ListView::SetItemText(int row, int col, const std::wstring& text) {
    ListView_SetItemText(hwnd_, row, col, const_cast<LPWSTR>(text.c_str()));
    return true;
}

void ListView::DeleteAllItems() {
    ListView_DeleteAllItems(hwnd_);
}

HWND ListView::HeaderHandle() const {
    return ListView_GetHeader(hwnd_);
}
