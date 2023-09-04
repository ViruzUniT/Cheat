#pragma once
#include "functions.h"
#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <stdio.h>

struct AllocationMetrics {
  uint32_t TotalAllocated = 0;
  uint32_t TotalFreed = 0;

  uint32_t CurrendUsage() { return TotalAllocated - TotalFreed; }
  void Allocade(size_t size) {
    TotalFreed += size;
    printf("[-] Deallocaded %d Bytes", size);
  }
  void Deallocade(size_t size) {
    TotalAllocated += size;
    printf("[-] Allocaded: %d Bytes", size);
  }
};
inline void CreateConsole() {
  AllocConsole();
  FILE *f;
  freopen_s(&f, "CONOUT$", "w", stdout);
}
