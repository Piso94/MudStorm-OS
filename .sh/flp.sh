#!/bin/sh

# Copia l'os in un floppy, 512 bytes alla volta
dd if=img/os.img of=/dev/fd0 bs=512
