/*  
 *  L.A. Riley 13-Feb-2000
 *  
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
*/

#define BUFFER_SIZE 50
#define ACK 6 

/* Danger Will Robinson, globals.
*/

unsigned char *receive_buffer;

/* Functions that read specific weather data */
int getInTemp     (int fd, int verbose, int debug_level);
int getInHiTemp   (int fd, int verbose, int debug_level);
int getInLoTemp   (int fd, int verbose, int debug_level);
int getOutTemp    (int fd, int verbose, int debug_level);
int getOutHiTemp  (int fd, int verbose, int debug_level);
int getOutLoTemp  (int fd, int verbose, int debug_level);
int getWindSpeed  (int fd, int verbose, int debug_level);
int getHiWindSpeed(int fd, int verbose, int debug_level);
int getWindDir    (int fd, int verbose, int debug_level);
int getPressure   (int fd, int verbose, int debug_level);
int getBarTrend   (int fd, int verbose, int debug_level); 
int getInHum      (int fd, int verbose, int debug_level);
int getInHiHum    (int fd, int verbose, int debug_level);
int getInLoHum    (int fd, int verbose, int debug_level);
int getOutHum     (int fd, int verbose, int debug_level);
int getOutHiHum   (int fd, int verbose, int debug_level);
int getOutLoHum   (int fd, int verbose, int debug_level);
int getRain       (int fd, int verbose, int debug_level);
int getDayRain    (int fd, int verbose, int debug_level);
int getWindChill  (int fd, int verbose, int debug_level);
int getLoWindChill(int fd, int verbose, int debug_level);
int getDewPoint   (int fd, int verbose, int debug_level);
int getHiDewPoint (int fd, int verbose, int debug_level);
int getLoDewPoint (int fd, int verbose, int debug_level);

/* Functions that read/display dates and times */
int getMonth(int date, int debug_level);
int getDay(int date, int debug_level);
int getHour(int time, int debug_level);
int getMinute(int time, int debug_level);
void printTimeStamp(int date, int time, int debug_level);

/* Serial interface functions */
int getData(int fd, int b, int c, int n, int debug_level);
int put_serial_char(int fd, int c, int debug_level);
int put_serial_string(int fd, char *s, int debug_level);
int send_unsigned(int fd, unsigned ui, int debug_level);
unsigned char get_serial_char(int fd, int debug_level);
static int get_acknowledge(int fd, int debug_level);
int fill_buffer(int fd, int n, int debug_level);
int fill_crc_buffer(int fd, int n, int debug_level);
    
