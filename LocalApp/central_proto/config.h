#ifndef _CONFIG
#define _CONFIG

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



/* ----------------------------------------------------------------- */
/* DEFINES */

#define ID_SIZE_NUM 128
#define NO_ERROR 0x00
#define ID_EXISTING_ERR 0x01
#define ERR_SYSTEM  0x02
#define CENTRAL_CHAR_REGOGNIZE 0x40 /* @ for bridge-central handshake */
#define CHAR_INFR 0x5F /* _ for notify the infringement*/
#define SWITCHENROLL_PIN 4
#define UNLOCKBUTTON_PIN 10
#define NOT_STRING "00000000000"
#define INFR_STRING "00000000001"
#define STANDARD_BOUNDRATE 9600
#define FINGERSENSOR_DATARATE 57600

/* -------------------------------------------------------------------*/



/* -------------------------------------------------------------------*/
/* GLOBAL VARIABLES */

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id = 0;
uint8_t enr = 0;
int read = 0;
int init_response = 0;
int check = 0;
const int rs=11, en=12, d4=5, d5=6, d6=7, d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int idarr[128]={0};
/* -------------------------------------------------------------------*/

#endif _CONFIG