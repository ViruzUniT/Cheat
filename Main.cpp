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

    Sleep(10);
  }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injected_thread, NULL, 0, NULL);
  }

  return EXIT_SUCCESS;
}
