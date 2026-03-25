#ifndef MYWINAPI_WINDOW_H
#define MYWINAPI_WINDOW_H

#include <windows.h>

class Window {
public:
    Window();

    bool Create(
        LPCWSTR className,
        LPCWSTR title,
        DWORD style,
        DWORD exStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int w = CW_USEDEFAULT,
        int h = CW_USEDEFAULT,
        HWND parent = nullptr,
        HMENU menu = nullptr
    );

    HWND Handle() const;

    static LRESULT CALLBACK WindowProc(
        HWND hwnd, UINT msg, WPARAM wp, LPARAM lp
    );
protected:
    virtual LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp);

private:
    HWND hwnd_;
};

#endif  // MYWINAPI_WINDOW_H
