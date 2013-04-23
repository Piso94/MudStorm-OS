MudStorm-OS
===========

My MicroOS written in Assembler x86/C

Prendendo spunto da DreamOS, le nuove versioni avranno un nome in codice (per capirci, un po' come Android)! :)

Versione OS: 0.6.1 orange (Arancia)

|------------------------------------------|
|COMPILAZIONE e VIRTUALIZZAZIONE MudStormOS|
|------------------------------------------|

Puo' essere virtualizzato con VirtualBox e QEMU (Bochs non l'ho provato), definendolo come floppy(l'immagine si trova nella cartella out)!

Per compilare ho usato Xubuntu 12.10, con gcc, nasm e ld! Come VirtualMachine ho usato QEMU! Se usate Linux, basta installare queste 4 cose, aprire il Terminale, andare alla cartella, e digitare './makes' o 'sh makes', e farà tutto da solo, vi chiederà la password di Root, per montare l'immagine floppy nella cartella tmp, copiare il file 'kernel.bin' al suo interno e smontare l'immagine!
Dopo di che se avete installato QEMU, partirà automaticamente!

Comando(Debian e derivate): sudo apt-get install nasm gcc qemu-system
N.B: ld viene installato con il pacchetto gcc
