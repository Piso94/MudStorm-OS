MudStorm-OS
===========

My MicroOS written in Assembler x86/C

Requisiti:
- CPU: x86
- RAM: 2MB (Richiesti)
- Keyboard
- Mouse (Anche se per ora non serve a molto)
- Floppy Drive

Prendendo spunto da DreamOS, le nuove versioni avranno un nome in codice (per capirci, un po' come Android)! :-)

In questa nuova versione ho aggiunto un piccolo easter egg! Provate a scrivere cowsay! :D Spero vi piaccia :-)

				 Versione OS: 0.6.3 Red Apple (Mela Rossa)

				|------------------------------------------|
				|	COMPILAZIONE e VIRTUALIZZAZIONE    |
				|        	MudStormOS		   |
				|------------------------------------------|

Puo' essere virtualizzato con VirtualBox, Bochs e QEMU, definendolo come floppy(l'immagine si trova nella cartella out)!

Per compilare ho usato Xubuntu 12.10, con gcc, nasm e ld! Come VM ho usato QEMU! Se usate Linux, basta installare queste 4 cose, aprire il Terminale, andare alla cartella, e digitare 'make' o 'make linux', e farà tutto da solo, vi chiederà la password di root, per montare l'immagine floppy nella cartella tmp, copiare il file 'kernel.bin' al suo interno e smontare l'immagine!
Dopo di che se avete installato QEMU, basterà dare il comando 'make qemu', o se volete copiarlo in un floppy: 'make floppy'
