#include <windows.h>
#include <stdio.h>

#include <jansson.h>

#define MOUSE_LOCATION_X(x) ((DWORD)x * (WORD)(-1) / GetSystemMetrics(SM_CXSCREEN))
#define MOUSE_LOCATION_Y(y) ((DWORD)y * (WORD)(-1) / GetSystemMetrics(SM_CYSCREEN))

/// @brief SendInput(Win32API) ラッパー
/// @param lpCmdLine INPUT構造体を模したJSON文字列
///
/// @code {.bat}
/// rundll32.exe SendInput.dll,_SendInput {"type":0,"mi":{"dx":0,"dy":0,"dwFlags":32769}}
/// @endcode
///
/// @li https://language-and-engineering.hatenablog.jp/entry/20081117/1226943698
/// @li http://jscript.zouri.jp/Source/MouseCtrl.html
/// @li https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
/// 
/// @note to compile: cl /LD .\SendInput.c user32.lib "C\:\lib\vcpkg\installed\x64-windows-static\lib\jansson.lib" /I "C\:\lib\vcpkg\installed\x64-windows-static\include"
__declspec(dllexport) void CALLBACK _SendInput(HWND hWnd, HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
{
    INPUT input;

    json_error_t error;
    json_t *root = json_loads(lpCmdLine, 0, &error);
    if(root == NULL) {
        // Invalid json
        return;
    }

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-input
    json_t *type = json_object_get(root, "type");
    if(type == NULL) {
        json_decref(root);
        return;
    }

    switch(json_integer_value(type))
    {
        case INPUT_MOUSE:
        {
            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
            input.type = INPUT_MOUSE;

            json_t *mi = json_object_get(root, "mi");
            if(mi == NULL) break;

            json_t *dx = json_object_get(mi, "dx");
            json_t *dy = json_object_get(mi, "dy");
            json_t *mouseData = json_object_get(mi, "mouseData");
            json_t *dwFlags = json_object_get(mi, "dwFlags");
            json_t *time = json_object_get(mi, "time");
            json_t *dwExtraInfo = json_object_get(mi, "dwExtraInfo");

            input.mi.dx = dx != NULL ? MOUSE_LOCATION_X(json_integer_value(dx)) : 0;
            input.mi.dy = dy != NULL ? MOUSE_LOCATION_Y(json_integer_value(dy)) : 0;
            input.mi.mouseData = mouseData != NULL ? json_integer_value(mouseData) : 0;
            input.mi.dwFlags = dwFlags != NULL ? json_integer_value(dwFlags) : 0;
            input.mi.time = time != NULL ? json_integer_value(time) : 0;
            input.mi.dwExtraInfo = dwExtraInfo != NULL ? json_integer_value(dwExtraInfo) : 0;

            SendInput(1, &input, sizeof(INPUT));

            json_decref(dx);
            json_decref(dy);
            json_decref(mouseData);
            json_decref(dwFlags);
            json_decref(time);
            json_decref(dwExtraInfo);
            json_decref(mi);

            break;
        }
        case INPUT_KEYBOARD:
        {
            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-keybdinput
            input.type = INPUT_KEYBOARD;

            json_t *ki = json_object_get(root, "ki");
            if(ki == NULL) break;

            json_t *wVk = json_object_get(ki, "wVk");
            json_t *wScan = json_object_get(ki, "wScan");
            json_t *dwFlags = json_object_get(ki, "dwFlags");
            json_t *time = json_object_get(ki, "time");
            json_t *dwExtraInfo = json_object_get(ki, "dwExtraInfo");

            input.ki.wVk = wVk != NULL ? json_integer_value(wVk) : 0;
            input.ki.wScan = wScan != NULL ? json_integer_value(wScan) : 0;
            input.ki.dwFlags = dwFlags != NULL ? json_integer_value(dwFlags) : 0;
            input.ki.time = time != NULL ? json_integer_value(time) : 0;
            input.ki.dwExtraInfo = dwExtraInfo != NULL ? json_integer_value(dwExtraInfo) : 0;

            SendInput(1, &input, sizeof(INPUT));

            json_decref(wVk);
            json_decref(wScan);
            json_decref(dwFlags);
            json_decref(time);
            json_decref(dwExtraInfo);
            json_decref(ki);

            break;
        }
        case INPUT_HARDWARE:
        {
            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-hardwareinput
            input.type = INPUT_HARDWARE;

            json_t *hi = json_object_get(root, "hi");
            if(hi == NULL) break;

            json_t *uMsg = json_object_get(hi, "uMsg");
            json_t *wParamL = json_object_get(hi, "wParamL");
            json_t *wParamH = json_object_get(hi, "wParamH");

            input.hi.uMsg = uMsg != NULL ? json_integer_value(uMsg) : 0;
            input.hi.wParamL = wParamL != NULL ? json_integer_value(wParamL) : 0;
            input.hi.wParamH = wParamH != NULL ? json_integer_value(wParamH) : 0;

            SendInput(1, &input, sizeof(INPUT));

            json_decref(uMsg);
            json_decref(wParamL);
            json_decref(wParamH);
            json_decref(hi);

            break;
        }
    }

    json_decref(type);
    json_decref(root);
}
