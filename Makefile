CFLAGS := -g -std=c99 -Wall -Wextra -Wpedantic
CFLAGS += -m32 -O1 -ffreestanding -fasm -fno-omit-frame-pointer
CPPFLAGS := -nostdinc -idirafter lib
LIBGCC := $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

SRCS := $(wildcard *.c) $(wildcard lib/*.c)
OBJS :=  $(patsubst src/,,$(SRCS:.c=.o)) # usar patsubst sobre SRCS

QEMU := qemu-system-i386 -serial mon:stdio
KERN ?= kern2
BOOT := -kernel $(KERN) $(QEMU_EXTRA)

kern2: boot.o stacks.o tasks.o funcs.o idt_entry.o $(OBJS)
	ld -m elf_i386 -Ttext 0x100000 $^ $(LIBGCC) -o $@
	# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS) -c $<

qemu: $(KERN)
	$(QEMU) $(BOOT)

qemu-gdb: $(KERN)
	$(QEMU) -kernel kern2 -S -gdb tcp:127.0.0.1:7508 $(BOOT)

gdb:
	gdb -q -s kern2 -n -ex 'target remote 127.0.0.1:7508'

clean:
	rm -f kern2 *.o lib/*.o core
	rm informe.toc
	rm informe.aux
	rm informe.log

informe:
	pdflatex informe.tex
	clear
code:
	./code.sh

.PHONY: qemu qemu-gdb gdb