// cycle_counter.c
#include <stdint.h>

// Function to read the RISC-V cycle counter
static inline uint32_t rdcycle(void) {
   uint32_t c;
   asm volatile("csrr %0, cycle" : "=r"(c));
   return c;
}
