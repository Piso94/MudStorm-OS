# make o make linux -> Compila i sorgenti
# make qemu -> Virtualizza l'os su VM QEMU
# make floppy -> Copia l'interno di out/os.img in un floppy

linux: 
	sh sh/makes.sh
initrd:
	sh sh/initrd.sh
qemu:
	sh sh/qemu.sh
floppy: 
	sh sh/flp.sh
