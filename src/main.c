#include <stdint.h>

typedef uint32_t u32;
#define sfr(a) ((volatile u32 *)(a))
#define gpiob sfr(0x58020400)
#define rcc sfr(0x58024400)
#define syst sfr(0xe000e010)
void reset() {
  rcc[0x38] = -1;
  rcc[0x3a] = 1;
  gpiob[0] &= 0x55555555;

  syst[1] = -1;
  syst[0] = 5;


  while (1) {
    gpiob[5] = syst[2] >> 8;
  }
}

void systick() {}
void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset, [15] = systick};
