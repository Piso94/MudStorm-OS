				|------------------------------------------|
				|	COMPILAZIONE e VIRTUALIZZAZIONE    |
				|        	MudStormOS		   |
				|------------------------------------------|

Puo' essere virtualizzato con VirtualBox, Bochs e QEMU, definendolo come floppy(l'immagine si trova nella cartella out)!
 _______________________________________________________________________________________
| 										   	|
| Se volete bootarlo da PC, basta che installate l'applicazione:		   	|
| - Windows == http://sourceforge.net/projects/win32diskimager/files/?source=navbar	|
| - Linux(Ubuntu, altri non testati) == sudo apt-get install usb-imagewriter	   	|
| - Mac: NaN! 									   	|
|									   	   	|
|  Dopo di che inserite una chiavetta vuota, la formattate,                        	|
|  selezionate l'img nella cartella principale,  					|
|  selezionate la chiavetta e cliccare su "writeimage", 			   	|
|  dopo di che, spegnete il PC, andate nel BIOS -> Boot,  			   	|
|  fatelo partire da chiavetta! Dopo di che salvare le modifiche e riavviare, 	   	|
|  dopo la schermata del bios, vi comparirà l'OS!		 		   	|
|  Il gioco e' fatto! :-)							   	|
|							   			   	|
|  Buon "divertimento"! ;-D								|
|											|
|  N.B: Su chiavette maggiori di 1GB il sistema non parte!				|
|       Non capisco dove sia il problema ;-(					   	|
|_______________________________________________________________________________________|

Per compilare ho usato Xubuntu 12.10, con gcc, nasm e ld! Come VM ho usato QEMU! Se usate Linux, basta installare queste 4 cose, aprire il Terminale, andare alla cartella, e digitare 'make' o 'make linux', e farà tutto da solo, vi chiederà la password di root, per montare l'immagine floppy nella cartella tmp, copiare il file 'kernel.bin' al suo interno e smontare l'immagine!
Dopo di che se avete installato QEMU, basterà dare il comando 'make qemu', o se volete copiarlo in un floppy: 'make floppy'
