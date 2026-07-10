AS = i686-elf-as
CC = i686-elf-gcc

CFLAGS = -std=c++17 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

OBJS = build/boot.o build/gdt_flush.o build/gdt.o build/idt_flush.o build/idt.o build/terminal.o build/kernel.o
 
all: build/bastOs.bin

build/boot.o: src/boot.s
	$(AS) src/boot.s -o build/boot.o

build/gdt_flush.o: src/gdt_flush.s
	$(AS) src/gdt_flush.s -o build/gdt_flush.o

build/gdt.o: src/gdt.cpp
	$(CC) -c src/gdt.cpp -o build/gdt.o $(CFLAGS)

build/idt_flush.o: src/idt_flush.s
	$(AS) src/idt_flush.s -o build/idt_flush.o

build/idt.o: src/idt.cpp
	$(CC) -c src/idt.cpp -o build/idt.o $(CFLAGS)

build/terminal.o: src/terminal.cpp
	$(CC) -c src/terminal.cpp -o build/terminal.o $(CFLAGS)

build/kernel.o: src/kernel.cpp
	$(CC) -c src/kernel.cpp -o build/kernel.o $(CFLAGS)

build/bastOs.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o build/bastOs.bin $(OBJS)

run: build/bastOs.bin
	qemu-system-i386 -kernel build/bastOs.bin -display cocoa,zoom-to-fit=on

clean:
	rm -f build/*.o build/*.bin
