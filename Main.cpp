#include "Base.h"

#define STATUS_INJECTED 0
#define STATUS_UNINJECTED 1

class Player {
public:
    char pad_0000[118]; //0x0000
    int8_t state; //0x0076
    int8_t visable; //0x0077
    char pad_0078[116]; //0x0078
    int32_t health; //0x00EC
    char pad_00F0[80]; //0x00F0
};

namespace GameFunctions {
    constexpr uintptr_t printf = 0x4DAD50;
    using printf_t = void(*)(const char* format, ...);
};

void SHOW_INJECTION_STATUS(const int status) {
    const auto printf = reinterpret_cast<GameFunctions::printf_t>(GameFunctions::printf);
    if (status == STATUS_INJECTED) {
        printf("Injected");
        MessageBoxW(
            NULL,
            L"Injected",
            L"MIAU",
            MB_OK | MB_ICONASTERISK
        );
    } else if (status == STATUS_UNINJECTED) {
        printf("Uninjected");
        MessageBoxW(
            NULL,
            L"Uninjected",
            L"MIAU",
            MB_OK | MB_ICONASTERISK
        );
    }
}

int GameLoop() {
    const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandle(L"ac_client.exe"));
    Player* player = *(reinterpret_cast<Player**>(client + 0x17E0A8));

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (GetAsyncKeyState(VK_INSERT))
            return EXIT_SUCCESS;

        player->health = 10;
    }
}

void injected_thread(const HMODULE instance) {
    SHOW_INJECTION_STATUS(STATUS_INJECTED);

    int Loop = GameLoop();

    // uninject
    SHOW_INJECTION_STATUS(STATUS_UNINJECTED);
    FreeLibraryAndExitThread(instance, EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        const auto thread= CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(injected_thread), hinstDLL, 0, NULL);

        if (thread) {
            CloseHandle(thread);
        }
    }
  
    return true;
}
