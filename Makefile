ARCH = -target armv7m-none-eabi -mcpu=cortex-m7 -flto
CC = clang
CXX = clang
FLAGS = $(ARCH) -O3 -Wall -MMD -MP -Iinc/ -ffreestanding -fno-exceptions -fno-unwind-tables -ffixed-point
CFLAGS = $(FLAGS) -std=c2y
CXXFLAGS = $(FLAGS) -std=c++2c

LD = clang -fuse-ld=lld
LDFLAGS = $(ARCH) -Tflash.ld -nostartfiles -nostdlib -static
LDFLAGS += -Wl,-X,-s,-S,--strip-all,--as-needed,--gc-sections,--icf=all,--lto-O3,--lto-whole-program-visibility,--exclude-libs=ALL

SRCS = $(wildcard src/*)
OBJS = $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))
OBJS := $(OBJS:.c=.o)
DEPS = $(OBJS:.o=.d)

TARGET = obj/main

all: $(TARGET).bin
	st-flash --connect-under-reset write $(TARGET).bin 0x08000000 

build: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	llvm-objcopy -O binary $< $@

$(TARGET).elf: $(OBJS) flash.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf obj/*

.PHONY: all build clean

