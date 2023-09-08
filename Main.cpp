#include "classes.h"
#include "debug.h"
#include "functions.h"
#include "gui.h"
#include "imgui/imgui.h"
#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <minwindef.h>
#include <stdio.h>
#include <thread>

#define STATUS_INJECTED 0
#define STATUS_UNINJECTED 1

void SHOW_INJECTION_STATUS(const int status) {
  if (status == STATUS_INJECTED) {
    GameFunctions::bigPrintf("Press Insert to uninject");
    GameFunctions::bigPrintf("Injected");
    // MessageBox(NULL, L"Injected", L"MIAU", MB_OK | MB_ICONASTERISK);
  } else if (status == STATUS_UNINJECTED) {
    GameFunctions::bigPrintf("Uninjected");
    // MessageBox(NULL, L"Uninjected", L"MIAU", MB_OK | MB_ICONASTERISK);
  }
}

GameClasses::Player *gcurrentPlayer = nullptr;
GameClasses::Player *GetCurrentPlayerPointer() { return gcurrentPlayer; }
void SetCurrentPlayerPointer(GameClasses::Player *currentPlayer) {
  gcurrentPlayer = currentPlayer;
}

int uninject() {
  gui::DestroyImGui();
  gui::DestroyDevice();
  gui::DestroyHWindow();

  return EXIT_SUCCESS;
}

char(__fastcall *O_Shoot)(DWORD *, int);
char __stdcall Shoot_Hook(DWORD *__this, int a2) {
  printf("%d", a2);
  return O_Shoot(__this, a2);
}

void FunctionManager(GameClasses::Player *&player,
                     GameClasses::MyPlayer &myPlayer) {
  // infinite Ammo
}

void RenderPlayer(GameClasses::Player *&player,
                  GameClasses::MyPlayer &myPlayer) {
  if (player->Name != myPlayer.Name)
    return;

  SetCurrentPlayerPointer(player);

  ImGui::Text("Player");
  ImGui::Checkbox("Invincible", &myPlayer.invincible);
  ImGui::SliderInt("Health", &player->health, 0, 1000, "%d",
                   ImGuiSliderFlags_AlwaysClamp);
  ImGui::Text("Coords");
  ImGui::Text("%f", player->body_coords.x);
  ImGui::SameLine();
  ImGui::Text("%f", player->body_coords.y);
  ImGui::SameLine();
  ImGui::Text("%f", player->body_coords.z);

  ImGui::Text("Pistol");
  ImGui::Checkbox("Infinite Clip", &myPlayer.pistol_infiniteAmmo);
  ImGui::SliderInt("Clip", &player->p_weapons->p_pistol->clip, 0, 1000, "%d",
                   ImGuiSliderFlags_AlwaysClamp);
  ImGui::SliderInt("Ammo", &player->p_weapons->p_pistol->ammo, 0, 1000, "%d",
                   ImGuiSliderFlags_AlwaysClamp);

  ImGui::Text("Gadgets");
  ImGui::SliderInt("Granates", &player->grenades, 0, 100, "%d",
                   ImGuiSliderFlags_AlwaysClamp);

  FunctionManager(player, myPlayer);
}

int GameLoop() {
  const uintptr_t client =
      reinterpret_cast<uintptr_t>(GetModuleHandleW(L"ac_client.exe"));

  GameClasses::Player *player =
      *(reinterpret_cast<GameClasses::Player **>(client + 0x17E0A8));
  GameClasses::PlayerList *playerList =
      *(reinterpret_cast<GameClasses::PlayerList **>(client + 0x18AC04));
  GameClasses::MyPlayer myPlayerList[32];
  const uint8_t *MatchSize = reinterpret_cast<uint8_t *>(client + 0x18AC0C);

  AllocConsole();
  FILE *f;
  freopen_s(&f, "CONOUT$", "w", stdout);

  // player + 205 = name
  // player array = ac_client.exe + 18AC04
  // match size = ac_client.exe + 18AC0C

  gui::CreateHWindow("Cube Cheat");
  gui::CreateDevice();
  gui::CreateImGui();

  while (gui::isRunning) {
    gui::BeginRender();
    // gui::Render();
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({gui::WIDTH, gui::HEIGHT});
    ImGui::Begin("Cube Menu", &gui::isRunning,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    if (playerList == nullptr && *MatchSize > 1) {
      playerList =
          *(reinterpret_cast<GameClasses::PlayerList **>(client + 0x18AC04));
    } else if (playerList != nullptr && *MatchSize < 1) {
      playerList = nullptr;
    }

    if (ImGui::CollapsingHeader("Player")) {
      if (ImGui::BeginTable("split", 1)) {
        ImGui::TableNextColumn();
        myPlayerList[0].Name = player->Name;

        RenderPlayer(player, myPlayerList[0]);
        ImGui::EndTable();
      }
    }

    if (playerList != nullptr && ImGui::CollapsingHeader("Players")) {
      if (ImGui::BeginTable("splitter", 1)) {
        ImGui::TableNextColumn();
        for (int i = 0; i < *MatchSize - 1; i++) {
          myPlayerList[i + 1].Name = playerList->Players[i]->Name;
          if (ImGui::CollapsingHeader(playerList->Players[i]->Name)) {
            if (ImGui::BeginTable(playerList->Players[i]->Name, 1)) {
              ImGui::TableNextColumn();
              RenderPlayer(playerList->Players[i], myPlayerList[i + 1]);
              ImGui::EndTable();
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
          }
        }
        ImGui::EndTable();
      }
    }

    // if (invincible && isAlive)
    //   player->health = 100;
    // if (pistol_infiniteAmmo && isAlive)
    //   player->p_weapons->p_pistol->clip = 10;

    ImGui::Text("Press Insert to");
    ImGui::SameLine();
    if (GetAsyncKeyState(VK_INSERT) || ImGui::Button("Uninject")) {
      ImGui::End();
      gui::EndRender();
      break;
    }

    ImGui::End();
    gui::EndRender();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  playerList = nullptr;
  player = nullptr;
  MatchSize = nullptr;

  printf("Uninjected\n");

  if (f != nullptr)
    fclose(f);
  FreeConsole();

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

    const HANDLE thread = CreateThread(
        nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(injected_thread),
        hinstDLL, 0, NULL);

    if (thread) {
      CloseHandle(thread);
    }
  }
  return true;
}
