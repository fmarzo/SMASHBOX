#ifndef _CONFIG
#define _CONFIG


//#define SIMULATION_MODE
//#define ASSIGN_ID_FROM_CENTRAL

#define NO_ERROR          0
#define INIT_ERR         -1
#define DEVICE_ID_ERR    -2
#define AHT_ERR          -3
#define ACCEL_ERR_RATE   -4
#define ACCEL_ERR_RANGE  -5
#define ACCEL_ERR_START  -5
#define ACCEL_ERR_UPDATE -6

/* NOTIFICATION CHAR */

#define OPEN_CHAR 0x26 /* & for opening lock */
#define INFR_CHAR 0x2A /* * for infringement lock */

/* SERIAL RATE */

#define STANDARD_BAUDRATE 9600

/* PINOUT */

#define PRESENCE_PIN 10
#define OPEN_PIN 11
#define MAGNET_PIN 12
#define LED_PIN 13

/* DEFINES */

#define BOX_OPENED 1
#define BOX_NOT_OPENED 0
String SIMULATION_STRING = "12154010";


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