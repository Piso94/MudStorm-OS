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
#include <string.h>
#include <stddef.h>
#include <kheap.h>

uint16_t *textmemptr = (uint16_t*)(0xb8000);
int attrib = 0x0F, csr_x, csr_y;

void scroll()
{
    size_t blank = 0x20 | (attrib << 8);
    size_t temp;

    if(csr_y >= 25)
    {
        temp = csr_y - 25 + 1;
        
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        
        csr_y = 24;
    }
}

void shell_csr_fix()
{
	csr_y--;
	move_csr(csr_x, csr_y);
}

void cls()
    /// Clear The screen
{
    size_t blank;

    blank = 0x20 | (attrib << 8);   // Make the "blank" (space + attribute)

    for(int i = 0; i < 25; i++)         // Do it on every Line of the screen
        memsetw (textmemptr + i * 80, blank, 80);  // Clear a line on the screen

    csr_x = 0;
    csr_y = 0;
    move_csr(csr_x, csr_y); // Push the change to the VGA controller
}

void set_color(char color)
{
	attrib = color;
}

void double_buffering(size_t x, size_t y, uint8_t color)
{

}

void putch(char c)
{
    uint16_t *where;
    size_t att = attrib << 8;

    if(c == 0x08)   // Handle a Backspace as a control charecter
    {
        if(csr_x != 0) 
	{
		csr_x--;					// Move it back one space
		where = textmemptr + (csr_y * 80 + csr_x);	// Find where we are
        	*where = ' ' | att;				// Make it blank.
	}
    }

    else if(c == 0x09)  // Handle a Tab
    {
        csr_x = (csr_x + 8) & ~(8 - 1); // Move the cursor forward modulus 8
    }
    
    else if(c == '\r')  // Carriage return
    {
        csr_x = 0;      // Return the cursor to the beginning of the screen
    }

    else if(c == '\n')  // Newline!
    {
        csr_x = 0;      // Carriage return is included in the newline
        csr_y++;        // Next line on the screen
    }

    else if(c >= ' ')   // Any ASCII non-Controll code:
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	// Character AND attributes: color
        csr_x++;
    }

    if(csr_x >= 80) // Check to see if we overflowed into next line
    {
        csr_x = 0;  // Adjust the line appropriately
        csr_y++;    // Increment line pointer
    }


    scroll();       // Check to see if we need to scroll the screen
    move_csr(csr_x, csr_y);     // Move the cursor to it's new home.
}

void puts(char *text)
{
    for (int i = 0; i < strlen(text); i++)
        putch(text[i]);
}

void move_csr(int x, int y)
{
    size_t temp;

    temp = y * 80 + x;  // Position = (y * width) +  x

    outportb(0x3D4, 14);        // CRT Control Register: Select Cursor Location
    outportb(0x3D5, (temp >> 8) & 0xFF); // Send the high byte across the bus
    outportb(0x3D4, 15);        // CRT Control Register: Select Send Low byte
    outportb(0x3D5, temp & 0xFF);      // Send the Low byte of the cursor location
}

bool detect_videotype()
{
	char c;
	
	c = ((*(volatile uint16_t*)0x410) & 0x30);
	return (c != 0x30); // false: Monochrome, true: Colour
}
