#ifndef _CONFIG
#define _CONFIG

/* DEFINES */
/* ----------------------------------------------------------------- */
#define OPEN_CHAR 0x26 /* & for opening lock */
#define INFR_CHAR 0x2A /* * for infringement lock */
#define PRESENCE digitalRead(4)
#define OPEN digitalRead(5)
/* ----------------------------------------------------------------- */

/* GLOBAL VARIABLES*/
/* ----------------------------------------------------------------- */
Adafruit_AHTX0 aht;
ADXL345 accel(ADXL345_ALT); //SDO low
double X, Y, Z;
sensors_event_t humidity, temp;
String lock = "0";
String infr, open;
String ID;
bool firstRun = true;  // Variabile per verificare la prima esecuzione
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