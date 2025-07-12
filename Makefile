ARCH = -target armv6m-none-eabi -mthumb -mcpu=cortex-m7
CXX = clang++
CXXFLAGS = $(ARCH) -O3 -Wall -std=c++2c -MMD -MP -Iinclude/ -ffreestanding 

LD = clang -fuse-ld=lld
LDFLAGS = $(ARCH) -Tflash.ld -nostartfiles -nostdlib -static
LDFLAGS += -Wl,-X,-s,-S,--as-needed,--gc-sections,--icf=all

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)
DEPS = $(OBJS:.o=.d)

TARGET = obj/main

all: $(TARGET).bin
	st-flash --connect-under-reset write $(TARGET).bin 0x08000000 

$(TARGET).bin: $(TARGET).elf
	llvm-objcopy -O binary $< $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf obj/*

.PHONY: all clean

