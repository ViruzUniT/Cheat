#pragma once

#include <cstdint>

namespace GameFunctions {
  constexpr uintptr_t big_printf = 0x45D890;
  using big_printf_t = void(*)(const char* format, ...);

  constexpr uintptr_t printf = 0x4DAD50;
  using printf_t = void(*)(const char* format, ...);
};
const GameFunctions::big_printf_t bigPrintf = reinterpret_cast<GameFunctions::big_printf_t>(GameFunctions::big_printf);
const GameFunctions::printf_t gPrintf = reinterpret_cast<GameFunctions::printf_t>(GameFunctions::printf);
