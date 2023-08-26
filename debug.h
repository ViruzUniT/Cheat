#pragma once
#include <cstdint>
#include "functions.h"

struct AllocationMetrics {
  uint32_t TotalAllocated = 0;
  uint32_t TotalFreed = 0;
  
  uint32_t CurrendUsage() { return TotalAllocated - TotalFreed; }
  void Allocade(size_t size) {
    TotalFreed += size;
    g_printf("[-] Deallocaded %d Bytes", size);
  }
  void Deallocade(size_t size) { 
    TotalAllocated += size;
    g_printf("[-] Allocaded: %d Bytes", size);
  }
};
