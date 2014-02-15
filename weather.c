/*  
 *  L.A. Riley 13-Feb-2000
 *  
 *  This is a set of functions for reading and displaying data 
 *  from a Davis Instruments Weather Monitor II equipped with 
 *  the WeatherLink RS232 serial interface.
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

#include <stdio.h>
#include "weather.h"
#include "crc-table.h"

/* ---------- Functions to read particular weather data values --------- */

int getInTemp(int fd, int verbose, int debug_level) {
  int temp, cal;
  
  temp = getData(fd, 0x44, 0x30, 2, debug_level);
  if(temp > 32768) {      /* Fix negative values. */
    temp = temp - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetInTemp: raw inside temp = %d\n", temp);
  }
  
  cal = getData(fd, 0x44, 0x52, 2, debug_level);
  if(cal > 32768) {      /* Fix negative values. */
    cal = cal - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetInTemp: inside temp calibration = %d\n", cal);
  }
  
  temp += cal;
  temp = (int)((float)5/9*(float)(temp - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Current Inside Temp [C]      : %6.1f\n", (float)temp/10);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetInTemp: Illegal verbose argument: %d\n",verbose); 
  }
  
  return temp;
}

int getInHiTemp(int fd, int verbose, int debug_level) {
  int temp;
  
  temp = getData(fd, 0x44, 0x34, 2, debug_level);
  if(temp > 32768) {      /* Fix negative values. */
    temp = temp - 65536; 
  }
  if (debug_level > 0) {
    printf("getInHiTemp: raw inside hi temp = %d\n", temp);
  }
  
  temp = (int)((float)5/9*(float)(temp - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("High Inside Temp [C]         : %6.1f", (float)temp/10);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0x44, 2, debug_level),
		   getData(fd, 0x44, 0x3C, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetInHiTemp: Illegal verbose argument: %d\n",
	    verbose); 
  }
  
  return temp;
}

int getInLoTemp(int fd, int verbose, int debug_level) {
  int temp;
  
  temp = getData(fd, 0x44, 0x38, 2, debug_level);
  if(temp > 32768) {      /* Fix negative values. */
    temp = temp - 65536; 
  }
  if (debug_level > 0) {
    printf("getInLoTemp: raw inside low temp = %d\n", temp);
  }
  
  temp = (int)((float)5/9*(float)(temp - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Low Inside Temp [C]          : %6.1f", (float)temp/10);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0x47, 2, debug_level),
		   getData(fd, 0x44, 0x40, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetInLoTemp: Illegal verbose argument: %d\n",
	    verbose); 
  }
  
  return temp;
}

int getOutTemp(int fd, int verbose, int debug_level) {
  int temp, cal;
  
  temp = getData(fd, 0x44, 0x56, 2, debug_level);
  if(temp > 32768) {      /* Fix negative values. */
    temp = temp - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetOutTemp: raw outside temp = %d\n", temp);
  }
  
  cal = getData(fd, 0x44, 0x78, 2, debug_level);
  if(cal > 32768) {      /* Fix negative values. */
    cal = cal - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetOutTemp: outside temp calibration = %d\n", cal);
  }
  
  temp += cal;
  temp = (int)((float)5/9*(float)(temp - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Current Outside Temp [C]     : %6.1f\n", (float)temp/10);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetOutTemp: Illegal verbose argument: %d\n",verbose); 
  }
  
  return temp;
}

int getOutHiTemp(int fd, int verbose, int debug_level) {
  int temp;
  
  temp = getData(fd, 0x44, 0x5A, 2, debug_level);
  if(temp > 32768) {      /* Fix negative values. */
    temp = temp - 65536; 
  }
  if (debug_level > 0) {
    printf("getOutHiTemp: raw outside hi temp = %d\n", temp);
  }
  
  temp = (int)((float)5/9*(float)(temp - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("High Outside Temp [C]        : %6.1f", (float)temp/10);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0x6A, 2, debug_level),
		   getData(fd, 0x44, 0x62, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetOutHiTemp: Illegal verbose argument: %d\n",
	    verbose); 
  }
  
  return temp;
}

int getOutLoTemp(int fd, int verbose, int debug_level) {
  int temp;
  
  temp = getData(fd, 0x44, 0x5E, 2, debug_level);
  if(temp > 32768) {      /* Fix negative values. */
    temp = temp - 65536; 
  }
  if (debug_level > 0) {
    printf("getOutLoTemp: raw outside low temp = %d\n", temp);
  }
  
  temp = (int)((float)5/9*(float)(temp - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Low Outside Temp [C]         : %6.1f", (float)temp/10);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0x6D, 2, debug_level),
		   getData(fd, 0x44, 0x66, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetOutLoTemp: Illegal verbose argument: %d\n",
	    verbose); 
  }
  
  return temp;
}

int getWindSpeed(int fd, int verbose, int debug_level) {
  int speed;
  
  speed = getData(fd, 0x22, 0x5E, 1, debug_level);
  if (debug_level > 0) {
    printf("getWindSpeed: raw wind speed = %d\n", speed);
  }
  
  speed = (int)(speed*1.609);  /* in m/s */
  
  if(verbose == 1) {
    printf("Current Wind Speed [m/s]     : %6d\n", speed);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetWindSpeed: Illegal verbose argument: %d\n",
	    verbose); 
  }
  
  return(speed);
}

int getHiWindSpeed(int fd, int verbose, int debug_level) {
  int speed;
  
  speed = getData(fd, 0x22, 0x60, 1, debug_level);
  if (debug_level > 0) {
    printf("getHiWindSpeed: raw high wind speed = %d\n", speed);
  }
  
  speed = (int)(speed*1.609);  /* in m/s */
  
  if(verbose == 1) {
    printf("High Wind Speed [m/s]        : %6d", speed);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x32, 0x68, 2, debug_level),
		   getData(fd, 0x42, 0x64, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetHiWindSpeed: Illegal verbose argument: %d\n",
	    verbose); 
  }
  
  return(speed);
}

int getWindDir(int fd, int verbose, int debug_level) {
  int direction;
  
  direction = getData(fd, 0x44, 0xB4, 2, debug_level);
  if (direction < 0 || direction > 360) {
    fprintf(stderr, "\ngetWindDir: Illegal wind direction: %d\n", direction);
  }
  if (debug_level > 0) {
    printf("getWindDir: raw wind direction = %d\n", direction);
  }
  
  if (verbose == 1) {
    printf("Current Wind Direction [deg] : %6d\n", direction);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetWindDir: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return(direction);
}

int getPressure(int fd, int verbose, int debug_level) {
  int pressure, cal;
  
  pressure = getData(fd, 0x44, 0x00, 2, debug_level); 
  if (debug_level > 0) {
    printf("getPressure: raw pressure = %d\n", pressure);
  }
  
  cal = getData(fd, 0x44, 0x2C, 2, debug_level);
  if (debug_level > 0) {
    printf("getPressure: pressure calibration = %d\n", cal);
  }
  
  pressure = (pressure - cal)*254;
  
  if (verbose == 1) {
    printf("Current Pressure [mm Hg]     : %6.0f\n", (float)pressure/100);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetPressure: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return pressure;
}

int getBarTrend(int fd, int verbose, int debug_level) {
  int trend, bits;
  char string[10]="Undefined!";

  bits = getData(fd, 0x12, 0x79, 1, debug_level); 

  if ((bits & 1) == 1) {
    trend = 0;
    strcpy(string, "Steady");
  } else if (bits == 2) {
    trend = 1;
    strcpy(string, "Rising");
  } else if (bits == 4) {
    trend = -1;
    strcpy(string, "Falling");
  } else {
    fprintf(stderr, "\ngetBarTrend: Illegal bit field: %d\n", (bits & 0x7));
  }

  if (verbose == 1) {
    printf("Barometric Trend             : %s\n", string);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetBarTrend: Illegal verbose argument: %d\n",
	    verbose);
  }

  return trend;
}

int getInHum(int fd, int verbose, int debug_level) {
  int hum;

  hum = getData(fd, 0x24, 0x80, 1, debug_level);
  if (hum == 128) {
    fprintf(stderr, "getInHum: inside sensor not reachable\n");
    return(-1);
  }
  if (debug_level > 0) {
    printf("getInHum: raw inside humidity = %d\n", hum);
  }

  if (verbose == 1) {
    printf("Current Inside Humidity [%%]  : %6d\n", hum);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetInHum: Illegal verbose argument: %d\n",
	    verbose);
  }

  return hum;
}

int getInHiHum(int fd, int verbose, int debug_level) {
  int hum;

  hum = getData(fd, 0x24, 0x82, 1, debug_level); 
  if (hum == 128) {
    fprintf(stderr, "getInHiHum: inside sensor not reachable\n");
    return(-1);
  }
  if (debug_level > 0) {
    printf("getInHiHum: raw inside humidity = %d\n", hum);
  }

  if (verbose == 1) {
    printf("High Inside Humidity [%%]     : %6d", hum);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0x8E, 2, debug_level),
		   getData(fd, 0x44, 0x86, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetInHiHum: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return hum;
}

int getInLoHum(int fd, int verbose, int debug_level) {
  int hum;
  
  hum = getData(fd, 0x24, 0x84, 1, debug_level); 
  if (hum == 128) {
    fprintf(stderr, "getInLoHum: inside sensor not reachable\n");
    return(-1);
  }
  
  if (debug_level > 0) {
    printf("getInLoHum: raw inside humidity = %d\n", hum);
  }
  
  if (verbose == 1) {
    printf("Low Inside Humidity [%%]      : %6d", hum);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0x91, 2, debug_level),
		   getData(fd, 0x44, 0x8A, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetInLoHum: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return hum;
}

int getOutHum(int fd, int verbose, int debug_level) {
  int hum, cal;
  
  hum = getData(fd, 0x24, 0x98, 1, debug_level);
  if (hum == 128) {
    fprintf(stderr, "getOutHum: outside sensor not reachable\n");
    return(-1);
  }
  if (debug_level > 0) {
    printf("getOutHum: raw outside humidity = %d\n", hum);
  }
  
  cal = getData(fd, 0x44, 0xDA, 2, debug_level);
  if (debug_level > 0) {
    printf("getOutHum: outside humidity cal number = %d\n", cal);
  }
  
  hum = hum + cal;   /* hum is now calibrated, but possibly out of range. */
  if (hum >100)
    hum = 100;
  else if (hum < 1)
    hum = 1;         /* hum has been clipped to the range 1 - 100. */
  
  if (verbose == 1) {
    printf("Current Outside Humidity [%%] : %6d\n", hum);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetOutHum: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return hum;
}

int getOutHiHum(int fd, int verbose, int debug_level) {
  int hum;
  
  hum = getData(fd, 0x24, 0x9A, 1, debug_level);
  if (hum == 128) {
    fprintf(stderr, "getOutHiHum: outside sensor not reachable\n");
    return(-1);
  }
  if (debug_level > 0) {
    printf("getOutHiHum: raw high outside humidity = %d\n", hum);
  }
  
  if (hum >100)
    hum = 100;
  else if (hum < 1)
    hum = 1;         /* hum has been clipped to the range 1 - 100. */
  
  if (verbose == 1) {
    printf("High Outside Humidity [%%]    : %6d", hum);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0xA6, 2, debug_level),
		   getData(fd, 0x44, 0x9E, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetHiOutHum: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return hum;
}

int getOutLoHum(int fd, int verbose, int debug_level) {
  int hum;

  hum = getData(fd, 0x24, 0x9C, 1, debug_level);
  if (hum == 128) {
    fprintf(stderr, "getOutLoHum: outside sensor not reachable\n");
    return(-1);
  }
  if (debug_level > 0) {
    printf("getOutLoHum: raw low outside humidity = %d\n", hum);
  }

  if (hum >100)
    hum = 100;
  else if (hum < 1)
    hum = 1;	      /* hum has been clipped to the range 1 - 100. */
  
  if (verbose == 1) {
    printf("Low Outside Humidity [%%]     : %6d", hum);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x34, 0xA9, 2, debug_level),
		   getData(fd, 0x44, 0xA2, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetLoOutHum: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return hum;
}

int getRain(int fd, int verbose, int debug_level) {
  int rain, cal;
  
  rain = getData(fd, 0x44, 0xCE, 2, debug_level);
  if (debug_level > 0) {
    printf("getRain: raw rain clicks = %d\n", rain);
  }
  
  cal = getData(fd, 0x44, 0xD6, 2, debug_level);
  if (debug_level > 0) {
    printf("getRain: rain calibration = %d\n", cal);
  }
  
  rain = (int)((float)rain / (float)cal * 254); /* in 0.1 mm */
  
  if (verbose == 1) {
    printf("Total Rain [cm]              : %6.3f\n", (float)rain/100);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetRain: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return rain;
}

int getDayRain(int fd, int verbose, int debug_level) {
  int rain, cal;
  
  rain = getData(fd, 0x44, 0xD2, 2, debug_level);
  if (debug_level > 0) {
    printf("getDayRain: raw rain clicks = %d\n", rain);
  }
  
  cal = getData(fd, 0x44, 0xD6, 2, debug_level);
  if (debug_level > 0) {
    printf("getDayRain: rain calibration = %d\n", cal);
  }
  
  rain = (int)((float)rain / (float)cal * 254); /* in 0.1 mm */
  
  if (verbose == 1) {
    printf("Daily Rain [cm]              : %6.3f\n", (float)rain/100);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetDayRain: Illegal verbose argument: %d\n",
	    verbose);
  }
  
  return rain;
}

int getWindChill  (int fd, int verbose, int debug_level) {
  int chill;

  chill = getData(fd, 0x42, 0xA8, 2, debug_level);
  if (debug_level > 0) {
    printf("\ngetChill: raw wind chill = %d\n", chill);
  }
  
  chill = (int)((float)5/9*(float)(chill - 320)); /* in units of 0.1 C */
  if (chill > 32768) {      /* Fix negative values. */
    chill = chill - 65536; 
  }

  /* xxx kludge alert 
     This is a kludge at best.  There is something more generally wrong 
     with the wind chill calculation, and we should figure-out how to 
     use the new formula recently published by NIST I think. 

     The problem is that we are often returning large negative numbers 
     which are rejected by the constraints on the readings table.  In 
     order to stop this I added this choke. 
  */
  if ((chill < -400) || (chill > 500))
    chill = 0;
  
  if(verbose == 1) {
    printf("Current Wind Chill [C]       : %6.1f\n", (float)chill/10);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetChill: Illegal verbose argument: %d\n",verbose); 
  }

  return chill;
}

int getLoWindChill  (int fd, int verbose, int debug_level) {
  int chill;

  chill = getData(fd, 0x42, 0xAC, 2, debug_level);
  if(chill > 32768) {      /* Fix negative values. */
    chill = chill - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetLoWindChill: raw low wind chill = %d\n", chill);
  }
  
  chill = (int)((float)5/9*(float)(chill - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Low Wind Chill [C]           : %6.1f", (float)chill/10);
    printf("  ("); 
    printTimeStamp(getData(fd, 0x32, 0xB4, 2, debug_level),
		   getData(fd, 0x42, 0xB0, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetLoWindChill: Illegal verbose argument: %d\n",
	    verbose); 
  }

  return chill;
}

int getDewPoint   (int fd, int verbose, int debug_level) {
  int dew;

  dew = getData(fd, 0x42, 0x8A, 2, debug_level);
  if(dew > 32768) {      /* Fix negative values. */
    dew = dew - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetDewPoint: raw dew point = %d\n", dew);
  }
  
  dew = (int)((float)5/9*(float)(dew - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Current Dew Point [C]        : %6.1f\n", (float)dew/10);
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetDewPoint: Illegal verbose argument: %d\n",verbose); 
  }

  return dew;
}

int getHiDewPoint(int fd, int verbose, int debug_level) {
  int dew;

  dew = getData(fd, 0x42, 0x8E, 2, debug_level);
  if(dew > 32768) {      /* Fix negative values. */
    dew = dew - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetHiDewPoint: raw high dew point = %d\n", dew);
  }
  
  dew = (int)((float)5/9*(float)(dew - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("High Dew Point [C]           : %6.1f", (float)dew/10);
    printf("  ("); 
    /* There appears to be an error in the Davis Techref.txt.
       The  DewLDate and DewHDate addresses seem to be swapped.
    */
    printTimeStamp(getData(fd, 0x32, 0x9E, 2, debug_level),
		   getData(fd, 0x42, 0x96, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetHiDewPoint: Illegal verbose argument: %d\n",
	    verbose); 
  }

  return dew;
}

int getLoDewPoint(int fd, int verbose, int debug_level) {
  int dew;

  dew = getData(fd, 0x42, 0x92, 2, debug_level);
  if(dew > 32768) {      /* Fix negative values. */
    dew = dew - 65536; 
  }
  if (debug_level > 0) {
    printf("\ngetLoDewPoint: raw low dew point = %d\n", dew);
  }
  
  dew = (int)((float)5/9*(float)(dew - 320)); /* in units of 0.1 C */
  
  if(verbose == 1) {
    printf("Low Dew Point [C]            : %6.1f", (float)dew/10);
    printf("  ("); 
    /* There appears to be an error in the Davis Techref.txt.
       The  DewLDate and DewHDate addresses seem to be swapped.
    */
    printTimeStamp(getData(fd, 0x32, 0xA1, 2, debug_level),
		   getData(fd, 0x42, 0x9A, 2, debug_level),
		   debug_level);
    printf(")\n");
  } else if(verbose != 0) {
    fprintf(stderr, "\ngetLoDewPoint: Illegal verbose argument: %d\n",
	    verbose); 
  }

  return dew;
}

/* ------------ Functions to decode/display data/time values ----------- */

int getMonth(int date, int debug_level) {
  int month;
  month = (date >> 8) & 017; // month is the last 4 bits of the first byte
  if(month < 1 || month > 12) {
    fprintf(stderr, "getMonth: Illegal month value: %d\n", month);
  }
  if(debug_level > 0) {
    printf("getMonth: month = %d\n", month);
  }
  return month;
}

int getDay(int date, int debug_level) {
  int day;
  day = date & 0377;                       // day is the second byte in BCD
  day = (int)((day >> 4) & 017)*10 + (int)(day & 017);
  if(day > 31 || day < 1) {
    fprintf(stderr, "getDay: Illegal day value: %d\n", day);
  }
  if(debug_level > 0) {
    printf("getDay: day = %d\n", day);
  }
  return day;
}

int getHour(int time, int debug_level) {
  int hour;
  hour = time & 0377;                       // hour is the second byte in BCD
  hour = (int)((hour >> 4) & 017)*10 + (int)(hour & 017);
  if(hour == 24) {
    hour = 0;
  } else if(hour > 24 || hour < 0) {
    fprintf(stderr, "getHour: Illegal hour value: %d\n", hour);
  }
  if(debug_level > 0) {
    printf("getHour: hour = %d\n", hour);
  }
  return hour;
}

int getMinute(int time, int debug_level) {
  int minute;
  minute = (time >> 8) & 0377;           // minute is the first byte in BCD
  minute = (int)((minute >> 4) & 017)*10 + (int)(minute & 017);
  if(minute > 60 || minute < 0) {
    fprintf(stderr, "getMinute: Illegal minute value: %d\n", minute);
  }
  if (debug_level > 0) {
    printf("getMinute: minute = %d\n", minute);
  }
  return minute;
}

void printTimeStamp(int date, int time, int debug_level) {
  int month, day, hour, minute;
  month  = getMonth(date, debug_level);
  day    = getDay(date, debug_level);
  hour   = getHour(time, debug_level);
  minute = getMinute(time, debug_level);

  if(month < 10) printf("0");
  printf("%d/",month);
  if(day < 10) printf("0");
  printf("%d ",day);
  if(hour < 10) printf("0");
  printf("%d:", hour);
  if(minute < 10) printf("0");
  printf("%d", minute);

  return;
}

/* -------- Serial I/O functions based on templates from Davis -------- */

int getData(int fd, int b, int c, int n, int debug_level) {
  int data;

  put_serial_string(fd, "WRD", debug_level); // Send read command.
  put_serial_char(fd, b, debug_level);       // address/size
  put_serial_char(fd, c, debug_level);       // data
  put_serial_char(fd, 0xd, debug_level);     // Send CR...
  get_acknowledge(fd, debug_level);          // verify that the command was 
                                             //    received an\d got ACK

  memset(receive_buffer, 0, BUFFER_SIZE);
  fill_buffer(fd, n, debug_level);  // get the returned data (in bytes)

  if(b == 0x44 || b == 0x42 || b == 0x34 || b == 0x32) {
    data = *((int*)receive_buffer);   // read raw data word
  } else if (b== 0x24 || b == 0x22 || b== 0x14 || b == 0x12) {
    data =(int) *((unsigned char*)receive_buffer);
  } else {
    fprintf(stderr, "\ngetData: Illegal size/bank argument.\n");
  }

  if(debug_level > 1) {
    printf("\ngetData: raw data word = %d\n", data);
  } 

  return data;
}

int put_serial_char(int fd, int c, int debug_level)  {
  unsigned char uc = c;

  if (debug_level > 1) {
    printf("put_serial_char: char = %x\n", uc);
  }
  
  return write(fd, &uc, 1);
}

int send_unsigned(int fd, unsigned ui, int debug_level) {
  unsigned char *uc;
  int j = 0; 
  
  uc = (unsigned char *)&ui;
  j += put_serial_char(fd, *uc, debug_level);
  j += put_serial_char(fd, *(uc+1), debug_level);
  
  return(j); 
}

int put_serial_string(int fd, char * s, int debug_level) {
  int i, j = 0;
  
  for (i = 0; s[i] != '\0'; i++)
    j += put_serial_char(fd, s[i], debug_level);
  
  return(j);
}

unsigned char get_serial_char(int fd, int debug_level) {
  unsigned char c;
  
  read(fd, &c, 1);
  
  if (debug_level > 1) {
    printf("get_serial_char: char = %x\n", c);
  }
  
  return(c);
}

static int get_acknowledge(int fd, int debug_level) {
  int c;
  
  if ((c = get_serial_char(fd, debug_level)) == ACK) {
    
    if (debug_level > 1) {
      printf("get_acknowledge: got ACK!\n");
    }
    
    return(1);   
    
  } else { 
    
    printf("get_acknowledge: Looking for ACK and got %x\n", c);
    return(0);
  }
}

int fill_buffer(int fd, int n, int debug_level) {
  int i,c;
  
  for (i=0; i<n; i++) {
    
    c = get_serial_char(fd, debug_level);
    
    if (debug_level > 1) {
      printf("fill_buffer: position = %d, value = %x\n", i, c);
    }
    
    if (c >= 0) {
      
      receive_buffer[i] = c;
      
    } else {
      
      printf("fill_buffer: error reading serial port\n");
      return(-1);
      
    }
  }
  
  return(1);
}

int fill_crc_buffer(int fd, int n, int debug_level) { // n includes CRC bytes
  int i,c;
  int crc = 0;   // initialize the CRC checksum to 0
  
  for (i=0; i<n; i++) {
    
    c = get_serial_char(fd, debug_level);
    
    if (debug_level > 1) {
      printf("fill_crc_buffer: position = %d, value = %x\n", i, c);
    }
    
    if (c >= 0) {
      
      receive_buffer[i] = c;   // Store the data
      crc = crc_table[(crc >> 8) ^ c] ^ (crc << 8);
      
    } else {
      
      printf("fill_crc_buffer: error reading serial port\n");
      return(-1);
    }
    
    if (crc == 0) {
      
      return(1);         // we have read in n bytes and the CRC is OK
      
    } else {
      
      printf("fill_crc_buffer: checksum error\n");
      return(-1);       // The CRC check failed.
    }
  }
}

