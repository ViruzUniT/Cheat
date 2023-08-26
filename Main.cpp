#include "imgui/imgui.h"
#include "gui.h"
#include <Windows.h>
#include <cstdint>
#include <thread>
#include "debug.h"
#include "functions.h"
#include "classes.h"

#define STATUS_INJECTED 0
#define STATUS_UNINJECTED 1

void SHOW_INJECTION_STATUS(const int status) {
  if (status == STATUS_INJECTED) {
    big_printf("Injected");
    //MessageBox(NULL, L"Injected", L"MIAU", MB_OK | MB_ICONASTERISK);
  } else if (status == STATUS_UNINJECTED) {
    big_printf("Uninjected");
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
  const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"ac_client.exe"));
  GameClasses::Player* player = *(reinterpret_cast<GameClasses::Player**>(client + 0x17E0A8));
  
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
    ImGui::SliderInt("Health", &player->health, 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);

    // ImGui::Text("Rifle");
    // ImGui::Checkbox("Infinite Clip", &rifle_infiniteAmmo);
    // ImGui::SliderInt("Rifle Clip", reinterpret_cast<int*>(&player->p_weapons->p_assault1->clip), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
    // ImGui::SliderInt("Rifle Ammo", reinterpret_cast<int*>(&player->p_weapons->p_assault1->ammo), 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);

    ImGui::Text("Pistol");
    ImGui::Checkbox("Infinite", &pistol_infiniteAmmo);
    ImGui::SliderInt("Pistol Clip", &player->p_weapons->p_pistol->clip, 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Pistol Ammo", &player->p_weapons->p_pistol->ammo, 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
        
    ImGui::Text("Gadgets");
    ImGui::SliderInt("Granates", &player->grenades, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp);

    if (invincible && isAlive) 
        player->health = 20;
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

  GameLoop();

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
