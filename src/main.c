#include <stdint.h>

typedef uint32_t u32;
#define sfr(a) ((volatile u32 *)(a))
#define tim2 sfr(0x40000000)
#define gpiob sfr(0x58020400)
#define rcc sfr(0x58024400)
#define syst sfr(0xe000e010)
#define iser sfr(0xe000e100)
#define halt while (1)
[[noreturn]] void reset() {
  rcc[10] = 2;
  rcc[12] = 4;
  *rcc = 0x01010001;
  rcc[4] = 3;


  rcc[0x38] = -1;
  rcc[0x3a] = 1;
  gpiob[0] &= 0x55555555;

  syst[1] = -1;
  syst[0] = 5;

  tim2[10] = 0;
  tim2[11] = 32e6;
  *tim2 = 1;
  tim2[3] = 1;
  iser[0] = 1 << 28;
  while (1) {
  }
}

void stack_top();
void nmi() { halt; }
void hardfault() { halt; }
void busfault() { halt; }
void usagefault() { halt; }
void systick() { gpiob[5] ^= -1; }
void tim2_int() {
  tim2[4] = 0;
  gpiob[5] ^= -1;
}
[[gnu::section(".text.ivt"), gnu::used]] void (*const ivt[])(void) = {
    stack_top,      reset,          nmi, hardfault, busfault, usagefault,
    [15] = systick, [44] = tim2_int};
