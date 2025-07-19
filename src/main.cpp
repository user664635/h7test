#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;

#define sfr(a) ((volatile u32 *)(a))
#define gpioa sfr(0x58020400)
#define rcc sfr(0x58024400)
#define syst sfr(0xe000e010)
extern "C" void reset() {
  rcc[0x38] = -1;
  gpioa[0] &= 0x55555555;

  syst[1] = -1;
  syst[0] = 5;

  while (1) {
    gpioa[5] = syst[2];
  }
}

extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset};
