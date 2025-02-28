#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include "check.h"
#include "enroll.h"
//NELLA VERSIONE ATTUALE PER SWITCHARE TRA LA MADOALITà DI REGISTRAZIONE (ENROLL) E DI VERIFICA, VIENE UTILIZZATO UN SENSORE A INFRAROSSI. 
// CHE IN BASE ALL'USCITA CHE RIPORTA FA SWITCHARE DA UNA MODALITà ALL'ALTRA

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


/* ----------------------------------------------- */

/* DEFINES */

#define ID_SIZE_NUM 128
#define NO_ERROR 0x00
#define ID_EXISTING_ERR 0x01
#define ERR_SYSTEM  0x02;
#define CENTRAL_CHAR_REGOGNIZE 0x40 /* @ for bridge-central handshake */
#define CHAR_INFR 0x5F /* _ for notify the infringement*/

/* ----------------------------------------------- */

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id = 0;
uint8_t ENR = 0;
int read = 0;
int init_response = 0;
int check = 0;

const int rs=11, en=12, d4=5, d5=6, d6=7, d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int idarr[128]={0};

/* DEBUG FUNCTION ONLY */

/*void find_and_add_id (void)
{
  int o = 0;
  for(int i=1; i < ID_SIZE_NUM; i++)
  {
    if (finger.loadModel(i) == FINGERPRINT_OK)
    {
      //Serial.print(i);
      //delay(500);
      idarr[o]=i;
      o++;
    }
  }
}
*/

void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  // set the data rate for the sensor serial port
  finger.begin(57600);

  pinMode(4, INPUT);
  pinMode(10,INPUT); // pulsante
  lcd.begin(16,1);
  lcd.setCursor(0,0);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  
  //finger.emptyDatabase(); //SE VUOI CANCELLARE TUTTI I TEMPLATE REGISTRATI 
 /* Sending a "0" to let the bridge recognize Central */

  Serial.print("00000000000"); 
 while (1)
  {
      if (Serial.available() > 0) 
      {
          init_response = Serial.read();
          lcd.print(init_response);
          if (init_response == CENTRAL_CHAR_REGOGNIZE)
          {
            
            lcd.clear();
            lcd.print("FOUND");
            break;
          }
      }
  }

 //if (finger.verifyPassword()) {
 //  Serial.println("Found fingerprint sensor!");
 //} else {
 //  Serial.println("Did not find fingerprint sensor :(");
 //  while (1) { delay(1); }
 //}
 //finger.getTemplateCount();

 //if (finger.templateCount == 0) {
 //  Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
 //}
 //else {
 //  Serial.println("Waiting for valid finger...");
 //    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
 //}

}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

int check_for_existing_id (uint8_t id)
{
  if (finger.loadModel(id) == FINGERPRINT_OK)
  {
      return ID_EXISTING_ERR;
  }
  
  return NO_ERROR;
}


/* --------------------- LOOP --------------------*/

void loop()                  
{

  int err = NO_ERROR;
  check = 0;
  ENR=0;
  
  //LEGGO SE IL BRIDGE MANDA IL LOCK 
  if (Serial.available() > 0) 
  {
    read = Serial.read();
    if (read == CHAR_INFR) 
    {
      //UTILIZZO COME MECCANISMO DI SBLOCCO IL SENSORE AD INFRAROSSI ALTRIMENTI NON ESCI DA LOOP
      while(1)
      {
        if (digitalRead(10) == LOW)
        {
          break;
        }
        Serial.print("00000000001"); 
      }
    } 
  }

  if (digitalRead(4) == LOW) 
  {
    ENR = 1;
  } 
  else 
  {
    ENR = 0;
  }

  if (ENR == 1)
  {
    lcd.clear();
    lcd.print("ENROLL");
    delay(500);

    id = random(1,127);

    while (check_for_existing_id (id) != NO_ERROR)
    {
      id = random(1,127);
    }

    lcd.clear();
    lcd.print("ENROLL: "+ String(id)); 
    delay(500);
    while (! getFingerprintEnroll() );
    delay(1000);
  }
  else
  {
    getFingerprintID();
    delay(50);   
  }

  if (check == 0){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("00000000000"); 
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }

}

