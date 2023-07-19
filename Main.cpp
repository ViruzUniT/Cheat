#include "Base.h"

#define STATUS_INJECTED 0
#define STATUS_UNINJECTED 1

class Vector3 {
public:
    float x, y, z;
};
class Vector2 {
public:
    float x, y;
};

class Player
{
public:
    char pad_0000[4]; //0x0000
    Vector3 coords; //0x0004
    Vector3 dir_velocity; //0x0010
    Vector3 unkown_vector; //0x001C
    Vector2 s_coords; //0x0028
    char pad_0030[70]; //0x0030
    uint8_t state; //0x0076
    uint8_t visable; //0x0077
    char pad_0078[116]; //0x0078
    uint32_t health; //0x00EC
    char pad_00F0[80]; //0x00F0
    uint32_t ammo; //0x0140
    char pad_0144[295]; //0x0144
}; //Size: 0x026B

namespace GameFunctions {
    constexpr uintptr_t big_printf = 0x45D890;
    using big_printf_t = void(*)(const char* format, ...);

    constexpr uintptr_t printf = 0x4DAD50;
    using printf_t = void(*)(const char* format, ...);
};

void SHOW_INJECTION_STATUS(const int status) {
    const GameFunctions::big_printf_t printf = reinterpret_cast<GameFunctions::big_printf_t>(GameFunctions::big_printf);
    if (status == STATUS_INJECTED) {
        printf("Injected");
        MessageBox(
            NULL,
            L"Injected",
            L"MIAU",
            MB_OK | MB_ICONASTERISK
        );
    } else if (status == STATUS_UNINJECTED) {
        printf("Uninjected");
        MessageBox(
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
    DWORD error;

    if (AllocConsole() == 0) {
        error = GetLastError();
        return EXIT_FAILURE;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (GetAsyncKeyState(VK_INSERT))
            return EXIT_SUCCESS;

        if (GetAsyncKeyState(VK_F1) && player->state == 0 && player->health >= 0)
            player->health = 100;
        
        std::cout << player->coords.x << " " << player->coords.y << " " << player->coords.z << " " << std::endl <<
            player->health << std::endl <<
            player->ammo << std::endl <<
            player->state << std::endl;
    }
}

void injected_thread(const HMODULE instance) {
    SHOW_INJECTION_STATUS(STATUS_INJECTED);

    int Loop = GameLoop();

    // uninject
    FreeConsole();
    FreeLibraryAndExitThread(instance, EXIT_SUCCESS);
    SHOW_INJECTION_STATUS(STATUS_UNINJECTED);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        const HANDLE thread= CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(injected_thread), hinstDLL, 0, NULL);

        if (thread) {
            CloseHandle(thread);
        }
    }
  
    return true;
}
