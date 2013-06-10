#!/bin/sh

## Creato da LittleHacker 2013
# Impostato qemu-system-i386 (funziona su 32bit e 64bit) al posto di qemu-system-x86_64 (solo su 64bit)
# Tolta la virtualizzazione, basta dare il comando make qemu

## NASM Compile
# nasm -f elf32 loader/<name_file_input(.S o .asm)> -o $build/<name_file_output.o>

## C Compile
# gcc $optcc <name_file_input.c> -o $build/<name_file_output.o>

## C++ Compile
# g++ $opt <name_file_input(.cc o .cpp) -o $build/<name_file_output.o>

## LD Link
# ld -m elf_i386 -T <name_file_link.ld> -o <name_file_output.bin> \
# 							$build/<name_files_input.o> ... \
#							... ...

# Pulisco lo schermo
clear

# Scrivo a video
echo "Setto i tools..."
# Setto la variabile PATH
export PATH=${PATH}:/usr/local/bin

# Setto le cartelle e gli argomenti per la compilazione
build=objs
img=img
temp=tmp

# Setto le opzioni di compilazione
optasm="-f elf32"
optcc="-m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -I./kernel/include -fno-strict-aliasing -fno-common -std=gnu99 -fno-stack-protector -c"
optld="-m elf_i386 -T linker/linker.ld"

# Scrivo a video
echo "Creo le cartelle..."
# Creo le cartelle
mkdir -p $build || exit
mkdir -p $temp || exit

echo "Compilazione (ASM) Loader..."
# ASM loader
nasm $optasm loader/loader.S -o $build/loader.o

echo "Compilazione (C) Kernel..."
# C kernel
gcc $optcc kernel/kernel.c -o $build/kernel.o

echo "Compilazione (C) Shell..."
# C shell
gcc $optcc kernel/shell/shell.c -o $build/shell.o
gcc $optcc kernel/shell/commands.c -o $build/commands.o

echo "Compilazione (C) Misc..."
# C misc
gcc $optcc kernel/misc/console.c -o $build/console.o
gcc $optcc kernel/misc/log.c -o $build/log.o
gcc $optcc kernel/misc/timer.c -o $build/timer.o
gcc $optcc kernel/misc/random.c -o $build/random.o
gcc $optcc kernel/misc/cpuid.c -o $build/cpuid.o

echo "Compilazione (C) Lib..."
# C lib
gcc $optcc kernel/lib/string.c -o $build/string.o
gcc $optcc kernel/lib/stdio.c -o $build/stdio.o

echo "Compilazione (C) Mm..."
# C mm
gcc $optcc kernel/mm/malloc.c -o $build/malloc.o
gcc $optcc kernel/mm/kheap.c -o $build/kheap.o

echo "Compilazione (C) Drivers..."
# C drivers
gcc $optcc kernel/drivers/kb.c -o $build/kb.o
gcc $optcc kernel/drivers/mouse.c -o $build/mouse.o
gcc $optcc kernel/drivers/video.c -o $build/video.o
gcc $optcc kernel/drivers/rtc.c -o $build/rtc.o 
gcc $optcc kernel/drivers/speaker.c -o $build/speaker.o
gcc $optcc kernel/drivers/io.c -o $build/io.o
gcc $optcc kernel/drivers/fpu.c -o $build/fpu.o

echo "Compilazione (C) int(e)r(rupt)..."
# C int(e)r(rupt)
gcc $optcc kernel/intr/gdt.c -o $build/gdt.o
gcc $optcc kernel/intr/idt.c -o $build/idt.o
gcc $optcc kernel/intr/irq.c -o $build/irq.o
gcc $optcc kernel/intr/isrs.c -o $build/isrs.o

echo "Linking..."
# LD linking
ld $optld -o kernel.bin \
	     $build/loader.o $build/kernel.o \
	     $build/shell.o $build/commands.o \
	     $build/console.o $build/io.o \
	     $build/random.o $build/timer.o $build/log.o $build/cpuid.o \
	     $build/stdio.o $build/string.o \
	     $build/malloc.o $build/kheap.o \
	     $build/kb.o $build/mouse.o $build/video.o $build/rtc.o $build/speaker.o $build/fpu.o \
	     $build/gdt.o $build/idt.o $build/irq.o $build/isrs.o

echo "Linking terminato"

echo "Compilazione terminata"

echo "Richiedo permessi per smontare la cartella \"tmp\" (per sicurezza)"
sudo umount $temp

echo "Richiedo permessi per montare la cartella \"tmp\""
sudo mount -o loop $img/os.img $temp || exit

sleep 0.5 || exit

echo "Richiedo permessi per copiare il file nella cartella \"tmp\""
sudo cp kernel.bin $temp/kernel.bin || exit

sleep 0.2 || exit

echo "Richiedo i permessi per smontare la cartella \"tmp\""
sudo umount $temp || exit

sleep 0.5 || exit

echo "Elimino il file *.bin"
rm kernel.bin || exit

echo "Elimino files *.o"
rm $build/*.o || exit

echo "Elimino le cartelle create"
rmdir $build || exit
rmdir $temp || exit

echo "Completato"

#echo "Avvio OS con QEMU"
#qemu-system-i386 -fda $img/os.img
