AS = i686-elf-as
CC = i686-elf-gcc

CFLAGS = -std=c++17 -ffreestanding -O2 -Wall -Wextra -fstack-protector-all -fno-exceptions -fno-rtti -Iinclude
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

# autamitically finding the files.
CXX_SOURCES = $(wildcard kernel/*.cpp mm/*.cpp drivers/*.cpp arch/i386/*.cpp)
ASM_SOURCES = $(wildcard arch/i386/*.s)


CXX_OBJS = $(patsubst %.cpp, build/%.o, $(notdir $(CXX_SOURCES)))
ASM_OBJS = $(patsubst %.s, build/%.o, $(notdir $(ASM_SOURCES)))

OBJS = $(ASM_OBJS) $(CXX_OBJS)

all: build/bastOs.bin

build/bastOs.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

#to find the files in the folders 
VPATH = kernel mm drivers arch/i386

build/%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

build/%.o: %.s
	$(AS) $< -o $@

run: build/bastOs.bin
	qemu-system-i386 -kernel build/bastOs.bin -display cocoa,zoom-to-fit=on

clean:
	rm -f build/*.o build/*.bin
