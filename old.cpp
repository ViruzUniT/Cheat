/*#include "Base.h"
#include <SDKDDKVer.h>

DWORD gProcId = GetProcId(L"ac_client.exe");
const uintptr_t gModuleBase = GetModuleBaseAddress(L"ac_client.exe", gProcId);
HANDLE gHProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, gProcId);

template <typename T>
T Read(T offset) {
    T value;
    if (!ReadProcessMemory(gHProcess, reinterpret_cast<LPVOID>(offset), &value, sizeof(value), nullptr))
        std::cerr << "fug" << std::endl;
    return value;
}

/*template <typename T, typename V>
void Read(T offset, V& value) {
    ReadProcessMemory(gHProcess, reinterpret_cast<LPVOID>(offset), &value, sizeof(value), nullptr);
}

template <typename T, typename V>
void Write(T offset, V value) {
    WriteProcessMemory(gHProcess, reinterpret_cast<LPVOID>(offset), &value, sizeof(value), nullptr);
}

class Player
{
public:
    char pad_0000[118]; //0x0000
    int8_t state; //0x0076
    int8_t visable; //0x0077
    char pad_0078[116]; //0x0078
    int32_t health; //0x00EC
    char pad_00F0[80]; //0x00F0
}; //Size: 0x0440

struct Vector3 { float x, y, z; };

class Player2
{
public:
    char pad_0000[4]; //0x0000
    Vector3 player_coords; //0x0004
    char pad_0010[312]; //0x0010
}; //Size: 0x0148

int main() {
    uintptr_t playerAddress = 0x57E0A8;

    int32_t health = 0;
    Player* player = (Player*)playerAddress; //gModuleBase + 0x17E0A8 = 0x57E0A8
    uintptr_t addr = 0; //FindDmaAddy(gHProcess, playerAddress, offsets);
    playerAddress = Read(playerAddress);
    player = Read(player);
    //health = GetHealth(player);
    health = Read(playerAddress + 0xEC);
    std::cout << health << " " << std::hex << playerAddress + 0xEC << std::endl << std::endl;

    health = 113;
    int32_t* health2 = Read(&player->health);
    std::stringstream ss;
    ss << std::hex << std::to_string((int)health2);
    int i;
    ss >> i;
    //health = *kawas

    std::cout << "Bitte es funkt: " << player << ", " << &player->health << ", " << Read(&player->health) << std::endl;

    Player2* player2 = (Player2*)playerAddress;
    player2 = Read(player2);
    Vector3* vector = Read(&player2->player_coords);
    std::cout << Read(vector) << std::endl;

    constexpr BYTE NORMAl_STATE = 0;
    constexpr BYTE INVISIBLE = 1;
    constexpr BYTE NOCLIP = 4;

    return 0;
}
*/