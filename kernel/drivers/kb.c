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

#include <video.h>
#include <io.h>
#include <kb.h>
#include <intr/irq.h>

unsigned char shift = 0;            	// Shift Key Status 
unsigned char ctrl = 0;             	// Ctrl Key Status
unsigned char alt = 0;              	// Alt Key Status 
unsigned char caps = 0;             	// Caps Lock Status
unsigned char num = 0;              	// Num Lock Status 
unsigned char keyBuff[257];         	// Keybuffer 
volatile unsigned char keyBuffEnd = 0;  // The Last key in the buffer
int line = 2;			    			// Line
unsigned char asciiCode;            	// The ASCII Code
unsigned char leds = 0;             	// The Three LED's on the keyboard.
bool in_chiaro = true;
unsigned char kbScanCodes[512] =    
{	
	// Normal				
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 0, '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0 , 0, '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, 0, 0, 0,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	// Shifted
	0, 27, '!', '"', 0, '$', '%', '&', '/', '(', ')' , '=', '?' , '^', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0 , 0, '\n', 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0, 0,
	'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, '7', '8', '9', 0, '4', '5', '6', 0,
	'1', '2', '3', '0', 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	// Caps, Normal 
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 0, '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0 , 0, '\n', 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0, 0,
	'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '-', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	// Caps, Shifted
	0, 27, '!', '"', 0, '$', '%', '&', '/', '(', ')', '=', '?', '^', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0 , 0, '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, 0, 0, 0,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
}; // Scancode Charatters



void FlushBuffer()
{
	unsigned temp;
	do
	{
		temp = inportb(0x64);
		if((temp & 0x01) != 0) 
		{
			(void)inportb(0x60);
			continue;
		}
	} while((temp & 0x02) != 0);
}


int kb_special(unsigned char key)
{	
	static int specKeyUp = 1;	// Is a key already been or being pressed?
	switch(key) 
	{
		case 0x36: // R-Shift down
		case 0x2A: // L-Shift down
			shift = 1;
			break;
		case 0xB6: // R-Shift up
		case 0xAA: // L-Shift up
			shift = 0;
			break;
		case 0x1D: // Control down
			ctrl = 1;
			break;
		case 0x9D: // Control up
			ctrl = 0;
			break;
		case 0x38: // Alt down
			alt = 1;
			break;
		case 0xB8: // Alt up
			alt = 0;
			break;
		case 0x3A: // Caps down
			if(specKeyUp == 1) 
			{
				caps = !caps;
				UpdateLeds(CapsLock);
				specKeyUp = 0;
			}
			break;
		case 0x45: // Num down
			if(specKeyUp == 1)
			{
				num = !num;
				UpdateLeds(NumLock);
				specKeyUp = 0;
			}
			break;
		case 0x46: // Scroll down
			if(specKeyUp == 1)
			{
				num = !num;
				UpdateLeds(ScrollLock);
				specKeyUp = 0;
			}
			break;
		case 0xBA: // Caps Up
		case 0xC5: // Num Up
		case 0xC6: // Scroll Up
			specKeyUp = 1;
			break;
		case 0xE0:
			break;
		default:
			return(0);
	}
	return (1);
}


void keyboard_handler(struct regs *r)
{
	unsigned char scanCode;
	scanCode = inportb(0x60);
	unsigned char asciiCode;
	
	
	if(!(kb_special(scanCode) | (scanCode >= 0x80)))
	{
		if(shift)	//Determine key that has been pressed
		{
			if(!caps)
			{
				asciiCode = kbScanCodes[scanCode + 128];
			}
			else
			{
				asciiCode = kbScanCodes[scanCode + 384];
			}
		}
		else
		{
			if(!caps)
			{
				asciiCode = kbScanCodes[scanCode];
			}
			else
			{
				asciiCode = kbScanCodes[scanCode + 256];
			}
		}

		keyBuffEnd++;
		keyBuff[keyBuffEnd] = asciiCode;
		if (keyBuff[keyBuffEnd] == '\b')
		{
			if ((line > 2) && (in_chiaro))
			{
				putch((int)keyBuff[keyBuffEnd]);
			}
		}
		else
		{
			if (in_chiaro)
			{
				putch((int)keyBuff[keyBuffEnd]);
			}
		}
	}
}

void keyboard_install()
{
	FlushBuffer();                      // Anything in the buffer is crap
	irq_install_handler(1, &keyboard_handler);  // Install Handler
}

void waitKey()
{
	FlushBuffer();  // Empty the Buffer of erratic data
	while (getchar() == 0);
}

void kbhit()
{
	in_chiaro = false;
}

void gets(char *s)
{
	init:
	line = line;
	int i;
	char k = (char)getchar();

	for (i = 0;k != '\n';i++)
	{
		if (k == '\b')
		{
			if (line > 2)
			{
				i--;
				s[i] = 0;
				i--;
				line--;
			}
			else
			{
				goto init;
			}			
			k = (char)getchar();
		}
		else
		{
			s[i] = k;
			line++;
			k = (char)getchar();
		}
	}
	line = 2;
	s[i] = '\0';
	in_chiaro = true;
}

char getchar_int()
{
	while(keyBuffEnd == 0);
	asm("cli");

	for(int i=0; i < keyBuffEnd; i++)
	{
		keyBuff[i] = keyBuff[i + 1];
	}
	keyBuffEnd--;

	asm("sti");

	return keyBuff[0];
}
int getchar()
{
	return ((int) getchar_int());
}

void UpdateLeds(char led)
{
	if(led == 0)
	{
		leds = 0;
	}
	else
	{
	if (leds == (leds|led))	        // If led is already on
		{
			leds = leds^led;	    // We turn the led off
		}
		else
		{
			leds = leds | led;	    // Else, turn led on
		}
	}
	while((inportb(0x64) &2) !=0);  // Loop until zero
	outportb(0x60, 0xED);
	while((inportb(0x64) &2) !=0 ); // Loop until zero
	outportb(0x60, leds);			// Update led status register
}