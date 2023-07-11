#include "Base.h"

class Player {
public:
    char pad_0000[118]; //0x0000
    int8_t state; //0x0076
    int8_t visable; //0x0077
    char pad_0078[116]; //0x0078
    int32_t health; //0x00EC
    char pad_00F0[80]; //0x00F0
};

namespace game {
    constexpr uintptr_t printf = 0x4DAD50;
    using printf_t = void(*)(const char* format, ...);
};

void injected_thread(const HMODULE instance) {
    const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandle(L"ac_client.exe"));
    
    const auto printf = reinterpret_cast<game::printf_t>(game::printf);
    printf("Injected");
    MessageBox(
        NULL,
        L"Injected",
        L"MIAU",
        MB_OK | MB_ICONASTERISK
    );

    Player* player = *(reinterpret_cast<Player**>(client + 0x17E0A8));
    while (!GetAsyncKeyState(VK_INSERT)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        player->health = 10;
    }

    // uninject
    MessageBox(
        NULL,
        L"Uninjected",
        L"MIAU",
        MB_OK | MB_ICONASTERISK
    );
    printf("uninjected");
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
