#pragma once

#include <cstdint>
#include <string>

struct Vector3 {
  float x, y, z;
};

namespace GameClasses {
struct MyPlayer {
  std::string Name;
  bool invincible;
  bool isAlive;
  bool rifle_infiniteAmmo;
  bool pistol_infiniteAmmo;

  MyPlayer()
      : invincible(false), isAlive(false), rifle_infiniteAmmo(false),
        pistol_infiniteAmmo(false) {}
};

class Player {
public:
  char pad_0000[4];          // 0x0000
  Vector3 head_coords;       // 0x0004
  Vector3 body_dir_velocity; // 0x0010
  Vector3 mbHeadvelocity;    // 0x001C
  Vector3 body_coords;       // 0x0028
  Vector3 rotation;          // 0x0034
  char pad_0040[54];         // 0x0040
  uint8_t state;             // 0x0076
  uint8_t visable;           // 0x0077
  char pad_0078[116];        // 0x0078
  int health;                // 0x00EC
  char pad_00F0[84];         // 0x00F0
  int grenades;              // 0x0144
  char pad_0148[189];        // 0x0148
  char Name[15];             // 0x0205
  char pad_0214[300];        // 0x0214
  class Weapons *p_weapons;  // 0x0340
  char pad_0344[2580];       // 0x0344
};                           // Size: 0x0D58

class Weapons {
public:
  char pad_0000[16];               // 0x0000
  class Pistol *p_pistol;          // 0x0010
  char pad_0014[140];              // 0x0014
  class AssaultRifle1 *p_assault1; // 0x00A0
  char pad_00A4[8];                // 0x00A4
};                                 // Size: 0x00AC

class PlayerList {
public:
  char pad_0000[4];
  Player *Players[31];
};

class Pistol {
public:
  int ammo;          // 0x0000
  char pad_0004[32]; // 0x0004
  int clip;          // 0x0024
  char pad_0028[32]; // 0x0028
};                   // Size: 0x0048
} // namespace GameClasses
