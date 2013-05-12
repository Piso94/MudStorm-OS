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

#include <stddef.h>
#include <shell.h>
#include <stdio.h>
#include <commands.h>
#include <string.h>
#include <video.h>
#include <log.h>
#include <console.h>

char current_dir[100] = "/";

struct cmd shellcmd[NUM] = { // Dichiaro una struttura con tutti questi comandi
{ "help", help, "Mostra i comandi" },
{ "clear", clear, "Pulisce lo schermo" },
{ "uname", uname, "Mostra le informazioni sull'OS. Es. uname -a" },
{ "ls", ls, "Mostra il contenuto della cartella corrente" },
{ "cd", cd, "Cambia cartella" },
{ "echo", eco, "Stampa a video quello che vuoi. Es. echo Ciao" },
{ "cowsay", cowsay, "Mostra una mucca" }, /* Questo non viene mostrato a video! E' l'easter egg */
{ "date", date, "Mostra la data" },
{ "beep", bee, "Suona col buzzer" },
{ "logo", logos, "Stampa a schermo il logo" },
{ "reboot", _reboot, "Spegne o riavvia la macchina" },
};

int find(string cmd)
{
	for(int i=0; i<NUM; i++) // Conto fino al valore della variabile NUM(definita nel file shell.h
	{
		if(!strcmp(shellcmd[i].cmdname, cmd)) 
		{
			return i; // Se il strcmp trova una stringa uguale sia nella stringa cmd, sia nella string shellcmd[i].cmdname, allora ritorna il numero in cui l'ha trovata
		}
	}
	return -1; // Altrimenti ritorna -1 (not found)
}

void runShell()
{
	logo(); // Stampo a video il logo
	Log.v("\t\t\t\t <help>\n"); // Scrivo a video

	while(1) // Ciclo infinito
	{
		char lst[100]; // Inizializzo la variabile lst

		for (int j=0;j<strlen(lst);j++)
			lst[j] = 0;

		static int s = 0;
		//printk("\n~$ "); // Scrive a video
		if (!s)
		{
			printk("\n%s~$ ", current_dir); // Scrivo a video la directory corrente
			s = 1;
		}
		else
		{
			s = 0;
		}
		scank("%s", lst); // Leggo quello che scrivi come una stringa

		char* first_command = strtok(lst, " "); // Splitto nella stringa first_command la stringa prima dello spazio
		argv = strtok(NULL, ""); // Splitto nella stringa argv la stringa dopo lo spazio

		int i = find(tolower(first_command)); // Inizializzo una variable "i" come "int" con valore "find(tolower(first_command))"

		if (i >= 0) // Controllo se la variabile "i" è maggiore o uguale a "0"
		{
			void (*commandJump)(void); // Inizializzo una variabile "*commandJump" come void
			commandJump = shellcmd[i].v_func; // Gli do valore shellcmd[i].v_func
			commandJump(); // Richiamo la funzione contenuta in shellcmd[i].v_func
		}
		else if ((i < 0) && (strcmp(first_command, "")))
		{
				printk("\nComando non trovato\n");
		}
	}
}
