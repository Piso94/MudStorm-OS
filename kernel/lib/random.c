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

#include <random.h>

static unsigned long int next = 1; // Inizializzo una variabile "next" come "static unsigned long int" con valore "1"
 
int rand(void)
{
    next = next * 1103515245 + 12345; // La variabile "next" è uguale a "next * 1103515245 + 12345"
    return (unsigned int)(next / 65536) % 32768; // Ritorno "(next / 65536) % 32768" come un "unsigned int"
}
 
void srand(unsigned int seed)
{
    next = seed; // La variabile "next" è uguale alla variabile "seed"
}
