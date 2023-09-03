#include <cstdint>
#include <malloc.h>
#include "debug.h"

#define ENABLE_LOG

#ifdef ENABLE_LOG
#  define LOG(x) x
#else
#  define LOG(x) (void) 0
#endif // ENABLE_LOG

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size) {
  if (size <= 0) {
    LOG(bigPrintf("[!] Memory Alloc failed: "));
    LOG(gPrintf("[!] size is 0"));
    return nullptr;
  }

  s_AllocationMetrics.Allocade(size);
  void* memoryLocation = malloc(size);
  if (memoryLocation && memoryLocation != nullptr)
    return memoryLocation;

  LOG(bigPrintf("[!] Memory Alloc failed"));
  LOG(gPrintf("[!] void* invalid or nullptr"));
  return nullptr;
}

void operator delete(void* memory, size_t size) {
  if (memory || memory == nullptr) {
    LOG(bigPrintf("[!] Memory Dealloc failed"));
    LOG(gPrintf("[!] void* is invalid or nullptr"));
    return;
  }

  if (size <= 0) {
    LOG(bigPrintf("[!] Memory Dealloc failed: "));
    LOG(gPrintf("[!] size is 0"));
    return;
  }

  s_AllocationMetrics.Deallocade(size);

  free(memory);
}
