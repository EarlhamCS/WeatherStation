/*  
 *  L.A. Riley 13-Feb-2000
 *  
 *  This is a greatly modified miniterm.c 
 *  by Sven Goldt (goldt@math.tu-berlin.de) 
 *
 *  This program reads the serial interface expecting to find a Davis 
 *  Systems WeatherLink interface.  This interface provides an RS232
 *  port to a weather station console made by the same company.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  13-Feb-2000   Lew Riley       First release, based on getWeather.05.c
*/

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "weather.h"

#define BAUDRATE B2400
#define SERIALDEVICE "/dev/ttyS1"
#define FALSE 0
#define TRUE 1

int main(int argc, char *argv[]) {
  struct termios oldtio, newtio, oldstdtio, newstdtio;
  int fd, debug_level = 0;
 
  /* Set the debug level.  0 = off, 1 = messages from weather value 
     functions, 2 = 1 plus messages from the serial interface functions.
  */ 
  if (argc < 2) {
    debug_level = 0; 
  } else {
    debug_level = atoi(argv[1]); 
  } 
  
  if (debug_level > 0) {
    printf("getWeather: debug_level = %d\n", debug_level); 
  }
  
  if ((receive_buffer = (unsigned char *)malloc(BUFFER_SIZE)) == NULL) {
    printf("getWeather: error allocating receive buffer (%d bytes).\n", 
	   BUFFER_SIZE);
    exit(-1);
  }
  
  /* Open serial port for reading and writing and not as controlling tty
     because we don't want to get killed if linenoise sends CTRL-C.  
  */
  fd = open(SERIALDEVICE, O_RDWR | O_NOCTTY);
  if (fd < 0) {
    printf("getWeather: error opening serial device.\n");
    perror(SERIALDEVICE); 
    exit(-1); 
  }
  
  /* Save the current serial port settings.
   */
  tcgetattr(fd, &oldtio); 
  
  /* Set bps rate and hardware flow control and 8n1 (8bit,no parity,1 stopbit).
     Also don't hangup automatically and ignore modem status.  Finally enable 
     receiving characters.
  */
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  
  /* Don't echo characters, we just want output from the device.  Don't 
     generate signals.  This is the quick and dirty way to do this, we should 
     use the specific masks for ECHO and ISIG but since we don't care about 
     having any of the items in this vector enabled we can just zero it.
  */
  newtio.c_lflag = 0;
  
  /* Ignore bytes with parity errors and leave port raw and dumb.
   */
  newtio.c_iflag = IGNPAR;
  
  /* Flush the I/O queue for the port and enable the new settings.
   */
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);
  
  /* Save the old settings and then stop echo and buffering on stdin.
   */
  tcgetattr(0, &oldstdtio);
  tcgetattr(0, &newstdtio); /* get working stdtio definition */
  newstdtio.c_lflag &= ~(ICANON | ECHO); /* change the flags */ 
  tcsetattr(0, TCSANOW, &newstdtio); /* change the configuration */ 
  
  /* Call the functions that collect the data from the Davis interface
     and write the results to stdout in metric form where appropriate.
  */
  
  printf("Station Time                 :          "); 
  printTimeStamp(getData(fd, 0x34, 0xC8, 2, debug_level), 
		 getData(fd, 0x44, 0xBE, 2, debug_level), 
		 debug_level); 
  printf("\n"); 
  
  getInHiTemp   (fd, 1, debug_level);
  getInLoTemp   (fd, 1, debug_level);
  getOutHiTemp  (fd, 1, debug_level);
  getOutLoTemp  (fd, 1, debug_level);
  getHiWindSpeed(fd, 1, debug_level);
  getInHiHum    (fd, 1, debug_level);
  getInLoHum    (fd, 1, debug_level);
  getOutHiHum   (fd, 1, debug_level);
  getOutLoHum   (fd, 1, debug_level);
  getLoWindChill(fd, 1, debug_level);
  getHiDewPoint (fd, 1, debug_level);
  getLoDewPoint (fd, 1, debug_level);

  /* Put the serial port and the terminal back the way the were when
     we started.
  */
  tcsetattr(fd, TCSANOW, &oldtio); 
  tcsetattr(0, TCSANOW, &oldstdtio); 
  
  close(fd);
  exit(0);
