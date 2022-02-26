#pragma once

#include <Windows.h>

namespace engine {

class Window {
public:
    HWND GetHandler() const {
        return handle_;
    }

    unsigned int GetWidth() const {
        RECT rect;
        GetWindowRect(handle_, &rect);
	    return rect.right - rect.left;
    }

    unsigned int GetHeight() const {
        RECT rect;
        GetWindowRect(handle_, &rect);
        return rect.top - rect.bottom;
    }

	Window(unsigned int width, unsigned int height)
	{
        HINSTANCE instance = GetModuleHandle(nullptr);
        RegisterWindowClass(instance);
        handle_ = CreateWindowInstance(instance, width, height);

        ShowWindow(handle_, SW_SHOW);
        SetForegroundWindow(handle_);
        SetFocus(handle_);
        ShowCursor(true);
	}

private:
    LPCWSTR window_name_ = L"GameWindow";
    HWND handle_;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
        return DefWindowProc(hwnd, umessage, wparam, lparam);
    }

    void RegisterWindowClass(HINSTANCE instance) const {
        WNDCLASSEX window_class;
        window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        window_class.lpfnWndProc = WndProc;        
        window_class.cbClsExtra = 0;
        window_class.cbWndExtra = 0;
        window_class.hInstance = instance;
        window_class.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
        window_class.hIconSm = window_class.hIcon;
        window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        window_class.lpszMenuName = nullptr;
        window_class.lpszClassName = window_name_;
        window_class.cbSize = sizeof(WNDCLASSEX);

        RegisterClassEx(&window_class);
    }

    HWND CreateWindowInstance(HINSTANCE instance, unsigned int width, unsigned int height) const {
        RECT window_rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
        AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

        return CreateWindowEx(
            WS_EX_APPWINDOW, 
            window_name_, 
            window_name_,
            WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME,
            (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
            window_rect.right - window_rect.left, 
            window_rect.bottom - window_rect.top, 
            nullptr, 
            nullptr, 
            instance, 
            nullptr);
    }
};

} // namespace engine