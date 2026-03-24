#include "Screen.hpp"

void Screen::Show() {
    ShowWindow(Handle(), SW_SHOW);
}

void Screen::Hide() {
    ShowWindow(Handle(), SW_HIDE);
}