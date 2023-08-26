#include <cstdint>
#include <corecrt_malloc.h>
#include "debug.h"

#define ENABLE_LOG

#ifdef ENABLE_LOG
#  define LOG(x) x
#else
#  define LOG(x) (void) 0
#endif // ENABLE_LOG

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size) {
  s_AllocationMetrics.Allocade(size);
  if(size > 0)
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
  s_AllocationMetrics.Deallocade(size);

  free(memory);
}
