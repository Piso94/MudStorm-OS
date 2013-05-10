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

#include <rtc.h>
#include <io.h>
#include <kmalloc.h>
 
#define CURRENT_YEAR 2013
                               
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t month;
size_t year;
 
enum 
{
      cmos_address = 0x70,
      cmos_data    = 0x71
};

int get_update_in_progress_flag()
{
      outportb(cmos_address, 0x0A);
      return (inportb(cmos_data) & 0x80);
}
 
 
uint8_t get_RTC_register(int reg)
{
      outportb(cmos_address, reg);
      return inportb(cmos_data);
}
 
 
void read_rtc()
{
      uint8_t century = (uint8_t)kmalloc(sizeof(uint8_t));
      uint8_t last_second;
      uint8_t last_minute;
      uint8_t last_hour;
      uint8_t last_day;
      uint8_t last_month;
      uint8_t last_year;
      uint8_t last_century;
      uint8_t registerB;
      int time[3];
      int century_register = 0x00;
 
      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates
 
      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      second = get_RTC_register(0x00);
      minute = get_RTC_register(0x02);
      hour = get_RTC_register(0x04);
      day = get_RTC_register(0x07);
      month = get_RTC_register(0x08);
      year = get_RTC_register(0x09);
      if (century_register != 0) 
      {
            century = get_RTC_register(century_register);
      }
 
      do {
            last_second = second;
            last_minute = minute;
            last_hour = hour;
            last_day = day;
            last_month = month;
            last_year = year;
            last_century = century;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            second = get_RTC_register(0x00);
            minute = get_RTC_register(0x02);
            hour = get_RTC_register(0x04);
            day = get_RTC_register(0x07);
            month = get_RTC_register(0x08);
            year = get_RTC_register(0x09);
            if (century_register != 0) 
            {
                  century = get_RTC_register(century_register);
            }
      } while( (last_second == second) && (last_minute == minute) && (last_hour == hour) &&
               (last_day == day) && (last_month == month) && (last_year == year) &&
               (last_century == century) );
 
      registerB = get_RTC_register(0x0B);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) 
      {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
            if (century_register != 0) 
            {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }
 
      // Convert 12 hour clock to 24 hour clock if necessary
 
      if (!(registerB & 0x02) && (time[2] & 0x80)) 
      {
            time[2] = ((time[2] & 0x7F) + 12) % 24;
      }
 
      // Calculate the full (4-digit) year
 
      if (century_register != 0) 
      {
            year += century * 100;
      } 
      else 
      {
            year += (CURRENT_YEAR / 100) * 100;
            if(year < CURRENT_YEAR) year += 100;
      }
      kfree(&century);
}
