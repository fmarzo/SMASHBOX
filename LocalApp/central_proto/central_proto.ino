#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include "check.h"


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

/* ----------------------------------------------- */

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id = 0;
uint8_t ENR = 0;
int read = 0;

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
  lcd.begin(16,1);
  lcd.setCursor(0,0);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
 randomSeed(analogRead(0));
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

  //finger.emptyDatabase(); //SE VUOI CANCELLARE TUTTI I TEMPLATE REGISTRATI 
  //LEGGO SE IL BRIDGE MANDA IL LOCK 
  if (Serial.available() > 0) {
    read = Serial.read();
    ENR=0;
    if (read == 49) {
      //UTILIZZO COME MECCANISMO DI SBLOCCO IL SENSORE AD INFRAROSSI ALTRIMENTI NON ESCI DA LOOP
      while(1){
        if (digitalRead(4) == LOW) {
          lcd.clear();
          lcd.print("Unlocked");
          break;
        }
        if (digitalRead(4) == HIGH){
          lcd.clear();
          lcd.print("ERROR");
          delay(1000);
          }
      }
    } 
    while (Serial.available()) {
      Serial.read();
    } //modo per pulire il buffer
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
}

/* ---------------------ENROLL --------------------*/

uint8_t getFingerprintEnroll() 
{
  int p = -1;
  lcd.clear();
  lcd.print("Waiting finger..");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("image taken");
      delay(500);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("Waiting finger..");
      delay(500);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Comm error");
      delay(500);
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging error");
      delay(500);
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(500);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image messy");
      delay(500);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("com error");
      delay(500);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("no finger feat");
      delay(500);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("no finger feat");
      delay(500);
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.print("Remove finger");
  delay(500);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();
  lcd.print("Put same finger");
  delay(500);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      delay(500);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print(".");
      delay(500);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("com error");
      delay(500);
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging error");
      delay(500);
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(500);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image messy");
      delay(500);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("comm error");
      delay(500);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("no finger feat");
      delay(500);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("no finger feat");
      delay(500);
      return p;
    default:
     // Serial.println("Unknown error");
      return p;
  }

  // OK converted!

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Prints matched");
    delay(500);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("com error");
    delay(500);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print("Prints not matched");
    delay(500);
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Stored!");
    delay(500);
    Serial.print("Stored: " + String(id));

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("error com");
    delay(500);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.print("can't store");
    delay(500);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.print("error");
    delay(500);
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }

  return true;
}

/* ---------------------CHECK --------------------*/


uint8_t getFingerprintID() 
{
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("image taken");
      delay(500);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("Waiting finger");
      delay(500);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("com error");
      delay(500);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("imaging error");
      delay(500);
      return p;
    default:
    //  Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(500);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image messy");
      delay(500);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("comm error");
      delay(500);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("no finger feat");
      delay(500);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("no finger feat");
      delay(500);
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("match found");
    delay(500);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("com error");
    delay(500);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.print("no match found");
    delay(500);
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }

  // found a match!
  lcd.clear();
  lcd.print("ID: " + finger.fingerID);
  delay(500);
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  lcd.clear();
  lcd.print("confidence: " + finger.confidence);
  delay(500);

  return finger.fingerID;
}
