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

#include <speaker.h>
#include <io.h>
#include <timer.h>

/* Speaker port enable I/O register. */
#define SPEAKER_PORT_GATE	0x61

/* Speaker port enable bits. */
#define SPEAKER_GATE_ENABLE	0x03

void sound(int frequency)
{
	if ((frequency >= 20) & (frequency <= 20000))
		outportb (SPEAKER_PORT_GATE, inportb (SPEAKER_PORT_GATE) | SPEAKER_GATE_ENABLE);
}


void nosound()
{
	outportb (SPEAKER_PORT_GATE, inportb (SPEAKER_PORT_GATE) & ~SPEAKER_GATE_ENABLE);
}

void beep()
{
	sound(440);
	delay_s(5);
	nosound();
}
