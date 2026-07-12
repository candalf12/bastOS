AS = i686-elf-as
CC = i686-elf-gcc

CFLAGS = -std=c++17 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

OBJS = build/boot.o build/gdt_flush.o build/keyboard.o build/gdt.o build/idt_flush.o build/idt.o build/isrs.o build/isr.o build/pic.o build/timer.o build/terminal.o build/kernel.o

all: build/bastOs.bin

# --- ARCHITECTURE (arch/i386) ---
build/boot.o: arch/i386/boot.s
	$(AS) arch/i386/boot.s -o build/boot.o

build/gdt_flush.o: arch/i386/gdt_flush.s
	$(AS) arch/i386/gdt_flush.s -o build/gdt_flush.o

build/gdt.o: arch/i386/gdt.cpp
	$(CC) -c arch/i386/gdt.cpp -o build/gdt.o $(CFLAGS)

build/idt_flush.o: arch/i386/idt_flush.s
	$(AS) arch/i386/idt_flush.s -o build/idt_flush.o

build/idt.o: arch/i386/idt.cpp
	$(CC) -c arch/i386/idt.cpp -o build/idt.o $(CFLAGS)

build/isrs.o: arch/i386/isrs.s
	$(AS) arch/i386/isrs.s -o build/isrs.o

build/isr.o: arch/i386/isr.cpp
	$(CC) -c arch/i386/isr.cpp -o build/isr.o $(CFLAGS)

# --- DRIVERS (drivers) ---
build/pic.o: drivers/pic.cpp
	$(CC) -c drivers/pic.cpp -o build/pic.o $(CFLAGS)

build/timer.o: drivers/timer.cpp
	$(CC) -c drivers/timer.cpp -o build/timer.o $(CFLAGS)

build/terminal.o: drivers/terminal.cpp
	$(CC) -c drivers/terminal.cpp -o build/terminal.o $(CFLAGS)

build/keyboard.o: drivers/keyboard.cpp
	$(CC) -c drivers/keyboard.cpp -o build/keyboard.o $(CFLAGS)
# --- KERNEL (kernel) ---
build/kernel.o: kernel/kernel.cpp
	$(CC) -c kernel/kernel.cpp -o build/kernel.o $(CFLAGS)

# --- LINKING & RUNNING ---
build/bastOs.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o build/bastOs.bin $(OBJS)

run: build/bastOs.bin
	qemu-system-i386 -kernel build/bastOs.bin -display cocoa,zoom-to-fit=on

clean:
	rm -f build/*.o build/*.bin
