#ifndef _CONFIG
#define _CONFIG

/* DEFINES */
/* ----------------------------------------------------------------- */
#define OPEN_CHAR 0x26 /* & for opening lock */
#define INFR_CHAR 0x2A /* * for infringement lock */
#define PRESENCE_PIN 4
#define OPEN_PIN 5
#define STANDARD_BOUNDRATE 9600
/* ----------------------------------------------------------------- */

/* GLOBAL VARIABLES*/
/* ----------------------------------------------------------------- */
Adafruit_AHTX0 aht;
ADXL345 accel(ADXL345_ALT); 
double X, Y, Z;
sensors_event_t humidity, temp;
String lock = "1";
String open;
String ID;
bool firstRun = true; 
int relock = 0; /* variable used to check if the box has been fisically opened and closed in order to reset the lock variable to 0 */
/* ----------------------------------------------------------------- */

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