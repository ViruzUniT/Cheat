#include "Base.h"

#define STATUS_INJECTED 0
#define STATUS_UNINJECTED 1

class Vector3 {
public:
    float x, y, z;
};

class Player
{
public:
    char pad_0000[4]; //0x0000
    Vector3 head_coords; //0x0004
    Vector3 body_dir_velocity; //0x0010
    Vector3 mbHeadvelocity; //0x001C
    Vector3 body_coords; //0x0028
    Vector3 rotation; //0x0034
    char pad_0040[54]; //0x0040
    uint8_t state; //0x0076
    uint8_t visable; //0x0077
    char pad_0078[116]; //0x0078
    uint32_t health; //0x00EC
    char pad_00F0[84]; //0x00F0 
    uint32_t grenades; //0x0144
    char pad_0148[504]; //0x0148
    class Weapons* p_weapons; //0x0340
    char pad_0344[2580]; //0x0344
}; //Size: 0x0D58

class Weapons
{
public:
    char pad_0000[16]; //0x0000
    class Pistol* p_pistol; //0x0010
    char pad_0014[140]; //0x0014
    class AssaultRifle1* p_assault1; //0x00A0
    char pad_00A4[8]; //0x00A4
}; //Size: 0x00AC

class AssaultRifle1
{
public:
    uint32_t ammo; //0x0000
    char pad_0004[32]; //0x0004
    uint32_t clip; //0x0024
    char pad_0028[28]; //0x0028
}; //Size: 0x0044

class Pistol
{
public:
    uint32_t ammo; //0x0000
    char pad_0004[32]; //0x0004
    uint32_t clip; //0x0024
    char pad_0028[32]; //0x0028
}; //Size: 0x0048

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
        //MessageBox(NULL, L"Injected", L"MIAU", MB_OK | MB_ICONASTERISK);
    } else if (status == STATUS_UNINJECTED) {
        printf("Uninjected");
        //MessageBox(NULL, L"Uninjected", L"MIAU", MB_OK | MB_ICONASTERISK);
    }
}

int uninject() {
    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}

int GameLoop() {
    const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandle(L"ac_client.exe"));
    Player* player = *(reinterpret_cast<Player**>(client + 0x17E0A8));
    
    bool isAlive = false;
    bool invincible = false;
    bool rifle_infiniteAmmo = false;
    bool pistol_infiniteAmmo = false;

    gui::CreateHWindow("Cube Cheat");
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::isRunning) {
        gui::BeginRender();
        //gui::Render();
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ gui::WIDTH, gui::HEIGHT });
        ImGui::Begin("Cube Menu", &gui::isRunning, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
        
        isAlive = player->health >= 0 && player->state != 1;
        
        ImGui::Text("Player");
        ImGui::Checkbox("Invincible", &invincible);
        ImGui::SliderInt("Health", reinterpret_cast<int*>(&player->health), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);

        ImGui::Text("Rifle");
        ImGui::Checkbox("Infinite Clip", &rifle_infiniteAmmo);
        ImGui::SliderInt("Rifle Clip", reinterpret_cast<int*>(&player->p_weapons->p_assault1->clip), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderInt("Rifle Ammo", reinterpret_cast<int*>(&player->p_weapons->p_assault1->ammo), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);

        ImGui::Text("Pistol");
        ImGui::Checkbox("Infinite", &pistol_infiniteAmmo);
        ImGui::SliderInt("Pistol Clip", reinterpret_cast<int*>(&player->p_weapons->p_pistol->clip), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderInt("Pistol Ammo", reinterpret_cast<int*>(&player->p_weapons->p_pistol->ammo), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);


        if (invincible && isAlive) 
            player->health = 20;
        if (rifle_infiniteAmmo && isAlive) 
            player->p_weapons->p_assault1->clip = 20;
        if (pistol_infiniteAmmo && isAlive) 
            player->p_weapons->p_pistol->clip = 10;

        if (GetAsyncKeyState(VK_INSERT) || ImGui::Button("Uninject")) {
            ImGui::End();
            gui::EndRender();
            return uninject();
        }

        ImGui::End();
        gui::EndRender();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return uninject();
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

        const HANDLE thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(injected_thread), hinstDLL, 0, NULL);

        if (thread) {
            CloseHandle(thread);
        }
    }
  
    return true;
}