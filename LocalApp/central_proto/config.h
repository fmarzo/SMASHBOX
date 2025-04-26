#ifndef _CONFIG
#define _CONFIG

#define __AVR__
/* ---------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SOFTWARE SERIAL PORT MANAGEMENT BASED ON THE DEVICES USED*/

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
/* For UNO and others without hardware serial, we must use software serial...*/
/* pin #2 is IN from sensor (GREEN wire)*/
/* pin #3 is OUT from arduino  (WHITE wire)*/
/*  Set up the serial port to use softwareserial..*/
SoftwareSerial mySerial(2, 3);
#else
/*  On Leonardo/M0/etc, others with hardware serial, use hardware serial!*/
/*  #0 is green wire, #1 is white*/
#define mySerial Serial1
/* If the microcontroller is an AVR (Arduino Uno, Nano, Pro Mini, etc.) or ESP8266, and it is NOT an Arduino Mega 2560, then a software serial port 
is created on pins 2 and 3 (SoftwareSerial mySerial(2, 3);).
Reason: These boards have only one hardware serial port (Serial), which is used for USB communication. If we want to communicate with another serial 
device (such as a fingerprint sensor), we need to create a software serial port using SoftwareSerial. */
#endif
/* ---------------------------------------------------------------------------------------------------------------------------------------------------*/

/* DEFINES */
#define MIN_ID_NUM 1
#define MAX_ID_NUM 127
#define PACKET_SIZE 7
#define ACTION_PACKET_SIZE 2
#define CENTRAL_RECOG 


/* SERIAL RATE */
#define STANDARD_BAUDRATE 9600
#define FINGER_SENSOR_DATARATE 57600

/* PINOUT */

#define SWITCH_ENROLL_PIN 4
#define UNLOCK_BUTTON_PIN 10

/* ERROR CODES */

#define NO_ERROR 0x00
#define ID_EXISTING_ERR 0x01
#define SYSTEM_ERROR  0x02

/* NOTIFICATION CHAR */

#define CENTRAL_RECOGNIZE_CHAR 0x40 /* @ for bridge-central handshake */
#define INFRING_CHAR  0x5F /* _ for notify the infringement*/

#endif _CONFIG