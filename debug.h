#pragma once
#include "functions.h"
#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string>

struct AllocationMetrics {
  uint32_t TotalAllocated = 0;
  uint32_t TotalFreed = 0;

  uint32_t CurrendUsage() { return TotalAllocated - TotalFreed; }
  void Allocade(size_t size) {
    TotalFreed += size;
    printf("[-] Deallocaded %zu Bytes", size);
  }
  void Deallocade(size_t size) {
    TotalAllocated += size;
    printf("Allocaded: %zu Bytes", size);
  }
};

inline void Blyat(FILE *f) {
  if (f == nullptr)
    fclose(f);
  FreeConsole();
}
