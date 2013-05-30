# make o make linux -> Compila i sorgenti
# make qemu -> Virtualizza l'os su VM QEMU
# make VBox -> Virtualizza l'os su VM VirtualBox
# make floppy -> Copia l'interno di out/os.img in un floppy

linux: 
	sh .sh/makes.sh
qemu:
	sh .sh/qemu.sh
vbox:
	sh .sh/vb.sh
floppy: 
	sh .sh/flp.sh
