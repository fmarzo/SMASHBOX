#ifndef _CONFIG
#define _CONFIG


/* NOTIFICATION CHAR */

#define OPEN_CHAR 0x26 /* & for opening lock */
#define INFR_CHAR 0x2A /* * for infringement lock */

/* SERIAL RATE */

#define STANDARD_BOUNDRATE 9600

/* PINOUT */

#define PRESENCE_PIN 4
#define OPEN_PIN 5
#define MAGNET_PIN 3

/* DEFINES */

#define BOX_OPENED 1
#define BOX_NOT_OPENED 0

#endif _CONFIG

/*
ERROR LEGEND for Debug
X1: read device id: failed
X2: could not find AHT
X3: write rate: failed
X4: write range: failed
X5: start: failed
X6: update failed
*/