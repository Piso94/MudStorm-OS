#!/bin/sh

# Virtualizza l'os su QEMU
echo "Virtualizzo l'OS con QEMU!"
qemu-system-i386 -fda img/os.img
