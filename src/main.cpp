#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;
#define gpioa (*(volatile u32 *)(0x58020014))
extern "C" void reset() {

  gpioa = 0xffffffff;
  while (1) {
  }
}

extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset};
