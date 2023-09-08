#include "debug.h"
#include <cstdint>
#include <iostream>
#include <malloc.h>
#include <stdio.h>

#define ENABLE_LOG

#ifdef ENABLE_LOG
#define LOG(x) x
#else
#define LOG(x) (void)0
#endif // ENABLE_LOG

static AllocationMetrics s_AllocationMetrics;

void *operator new(size_t size) {
  if (size <= 0) {
    LOG(printf("[!] Memory Alloc failed: size is 0"));
  }

  s_AllocationMetrics.Allocade(size);
  void *memoryLocation = malloc(size);
  if (memoryLocation && memoryLocation != nullptr)
    return memoryLocation;

  LOG(printf("[!] Memory Alloc failed: void* invalid or nullptr"));
  throw "bye";
}

void operator delete(void *memory, size_t size) {
  if (memory || memory == nullptr) {
    LOG(printf("[!] Memory Dealloc failed: void* invalid or nullptr"));
    return;
  }

  if (size <= 0) {
    LOG(printf("[!] Memory Dealloc failed: size is 0"));
    return;
  }

  s_AllocationMetrics.Deallocade(size);

  free(memory);
}
