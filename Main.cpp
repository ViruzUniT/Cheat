#include "Base.h"

class Player
{
public:
    char pad_0000[118]; //0x0000
    int8_t state; //0x0076
    int8_t visable; //0x0077
    char pad_0078[116]; //0x0078
    int32_t health; //0x00EC
    char pad_00F0[80]; //0x00F0
};


void injected_thread() {
    Player* player = *(reinterpret_cast<Player**>(0x57E0A8));
    while (true) {
        player->health = 10;

        // So our thread doesn't constantly run, we have it pause execution for a millisecond.
        // This allows the processor to schedule other tasks.
        Sleep(10);
    }
}

// When injected, the parent process looks for the DLL's DllMain, similar to the main function in regular executables.
// There are several events that can occur, the most important one for us being DLL_PROCESS_ATTACH. This occurs when the
// DLL is fully loaded into the process' memory.
//
// Once loaded, we create a thread. This thread will run in the background of the game as long as the process remains open.
// The code that this thread will execute is shown above.
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injected_thread, NULL, 0, NULL);
    }

    return true;
}