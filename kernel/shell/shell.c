/*
 * MudStorm OS LittleHacker
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <types.h>
#include <console.h>
#include <video.h>
#include <shell.h>
#include <stdio.h>
#include <rtc.h>
#include <delay.h>
#include <version.h>
#include <string.h>
#include <speaker.h>
#include <log.h>

int DEBUG = 1; // Inzializzo una variabile "int" con nome "DEBUG" con valore "1"
char lst[100]; // Inizializzo una variabile "char array" con nome "lst" e caratteri(max.) "100"
char* second_command = NULL; // Inzializzo una variabile "char*" con nome "second_command" con valore "NULL"

void help()
{
	if (DEBUG) Log.d("\nScrivo i comandi possibili a video\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	for (int i=0;i<NUM;i++) // Conto fino al valore della variabile NUM(definita nel file shell.h)
	{
	printk("\n %s - %s", shellcmd[i].cmdname, shellcmd[i].cmddesc); // Scrivo a schermo: nome del comando - descrizione
	}
	printk("\n"); // Vado a capo
}

void clear()
{
	if (DEBUG) Log.d("\nPulisco lo schermo"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	cls(); // Pulisco lo schermo
	if (DEBUG) Log.d("\nStampo a video il logo\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	logo(); // Stampo a video il logo
}

void uname()
{
	if (DEBUG) Log.d("\nStampo a video le informazioni dell'OS\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	if (!strcmp(second_command, "-a"))
	{
		printk("\n%s%s, %s: %s - %s: %s\n%s: %s\n", "MudStormOS", REVISION, "Kernel", KERNEL, "Shell", SHELL, "Autore", AUTHOR); // Scrivo a schermo "MudStormOS-Revisione, Kernel:Versione - Shell:Versione a capo Autore: Autore"
	}
	else if (!strcmp(second_command, "-r"))
	{
		printk("\n%s%s\n", "MudStormOS", REVISION); // Scrivo a schermo "MudStormOS-Revisione"
	}
	else if (!strcmp(second_command, "-k"))
	{
		printk("\n%s: %s\n", "Kernel", KERNEL); // Scrivo a schermo "Kernel:Versione"
	}
	else if (!strcmp(second_command, "-s"))
	{
		printk("\n%s: %s\n", "Shell", SHELL); // Scrivo a schermo "Shell:Versione"
	}
	else if (!strcmp(second_command, "-n"))
	{
		printk("\n%s: %s\n", "Autore", AUTHOR); // Scrivo a schermo "Autore: Autore"
	}
	else
	{
		printk("\nUsa:\n -a (Vedi tutte le info)\n -r (Vedi numero di revisione)\n -k (Vedi versione kernel)\n -s (Vedi versione shell)\n -n (Vedi nome autore)\n");
	}
}

void eco()
{
	if (!strcmp(second_command, NULL)) // Controllo se la stringa in second_command
		printk("\n"); // Se è NULL allora scrivo solo un "a capo"
	else 
		printk("\n%s\n", second_command); // Altrimenti scrivo la stringa contenuta
}

void date()
{
	if (DEBUG) Log.d("\nLeggo le informazioni dall'RTC"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	read_rtc(); // Leggo dall'RTC
	if (DEBUG) Log.d("\nScrivo le informazioni ricevute dall'RTC\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	printk("\n %d:%d:%d - %d/%d/%d\n", hour, minute, second, day, month, year); 
	// Scrivo a schermo "ora:minuti:secondi - giorno/mese/anno"
}

void cpu()
{
	if (DEBUG) Log.d("\nScrivo a video la velocità (attuale) della CPU\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	int speed = cpu_speed(); // Inzializzo una variabile "speed" come "int" con valore(preso da una funzione) "cpu_speed()"
	printk("\n%d (MHz)\n", speed); // Scrivo a video
}

void beepp()
{
	if (DEBUG) Log.d("\nFaccio suonare il buzzer\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	beep(); // Richiamo la funzione beep();
}

void debug()
{
	if (DEBUG) // Se la variabile "DEBUG" ha valore "1"
	{
		DEBUG = 0; // "DEBUG" cambia valore
		Log.d("\nDebug disabilitato\n"); // Scrivo a video
	}
	else // Se la variabile "DEBUG" ha valore "0"
	{
		DEBUG = 1; // "DEBUG" cambia valore
		Log.d("\nDebug abilitato\n"); // Scrivo a video
	}
}

void logos()
{
	if (DEBUG) Log.d("\nStampo a video il logo"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	logo(); // Richiamo la funzione logo();
}

void reb()
{
	if (DEBUG) Log.d("\nRiavvio la macchina\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	printk("Riavvio...Sicuro[S/n]? "); // Scrivo a video
	char risposta[2]; // Inizializzo la stringa risposta!
	scank("%s", risposta); // Leggo la stringa che inserirai
	if (!strcmp(tolower(risposta), "s")) // Se risposta è uguale a "s"
	{
		reboot(); // Richiamo la funzione reboot();
	}
}

void shut()
{
	if (DEBUG) Log.d("\nArresto la macchina\n"); // Se la variabile "DEBUG" ha valore "1", allora scrivi a schermo
	printk("Spengo...Sicuro[S/n]? "); // Scrivo a video
	char risposta[2]; // Inizializzo la stringa risposta!
	scank("%s", risposta); // Leggo la stringa che inserirai
	if (!strcmp(tolower(risposta), "s")) // Se risposta è uguale a "s"
	{
		shutdown(); // Richiamo la funzione shutdown();
	}
}

struct cmd shellcmd[NUM] = { // Dichiaro una struttura con tutti questi comandi
{ "help", help, "Mostra i comandi" },
{ "clear", clear, "Pulisce lo schermo" },
{ "uname", uname, "Mostra le informazioni sull'OS" },
{ "echo", eco, "Stampa a video quello che scrivi dopo lo spazio" },
{ "date", date, "Mostra la data" },
{ "cpu", cpu, "Mostra la velocita' attuale della CPU" },
{ "beep", beepp, "Suona col buzzer" },
{ "debug", debug, "Controlla cosa fa la shell" },
{ "logo", logos, "Stampa a schermo il logo" },
{ "reboot", reb, "Riavvia la macchina" },
{ "shutdown", shut, "Arresta la macchina" },
};

int find(string cmd)
{
	for(int i=0;i<NUM;i++) // Conto fino al valore della variabile NUM(definita nel file shell.h
	{
		if(!strcmp(shellcmd[i].cmdname, cmd)) 
			return i; // Se il strcmp trova una stringa uguale sia nella stringa cmd, sia nella string shellcmd[i].cmdname, allora ritorna il numero in cui l'ha trovata
	}
	return -1; // Altrimenti ritorna -1 (not found)
}

void runShell()
{
	//cls(); // Pulisco lo schermo
	logo(); // Stampo a video il logo
	set_color(BAK_WHITE); // Le prossime scritte saranno nere con sfondo bianco
	printk("\t\t\t\t<help>\n"); // Scrivo a video
	set_color(WHITE); // Le prossime scritte saranno bianche
	DEBUG = 0;
	
	while(1) // Ciclo infinito
	{
		for(int i=0;i<101;i++) lst[i] = '\0'; // Resetto l'array di char
		printk("\n> "); // Scrivo a video
		scank("%s", lst); // Leggo quello che scrivi come una stringa
		char* first_command = strtok(lst, " "); // Splitto nella stringa first_command la stringa prima dello spazio
		second_command = strtok(NULL, ""); // Splitto nella stringa second_command la stringa dopo lo spazio
		int i = find(tolower(first_command)); // Inizializzo una variable "i" come "int" con valore "find(tolower(first_command))"
		if (i >= 0) // Controllo se la variabile "i" è maggiore o uguale a "0"
		{
			void (*commandJump)(void); // Inizializzo una variabile "*commandJump" come void
			commandJump = shellcmd[i].v_func; // Gli do valore shellcmd[i].v_func
			commandJump(); // Richiamo la funzione contenuta in shellcmd[i].v_func
		}
		else
		{
			if (DEBUG) Log.d("\nComando non trovato\n");
		}
	}
}
