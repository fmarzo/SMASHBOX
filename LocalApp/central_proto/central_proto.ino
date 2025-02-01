
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

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


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id, ENR;
int read;
const int rs=11, en=12, d4=5, d5=6, d6=7, d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

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

}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  //finger.emptyDatabase(); //SE VUOI CANCELLARE TUTTI I TEMPLATE REGISTRATI 
  //LEGGO SE IL BRIDGE MANDA IL LOCK 
  if (Serial.available() > 0) {
    read = Serial.read();
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

  if (digitalRead(4) == LOW) {
    ENR = 1;
  } else {
    ENR = 0;
  }
  if(ENR == 1){
    Serial.print("ENROLL");
    lcd.clear();
    lcd.print("ENROLL");
    lcd.clear();
    lcd.print("Type ID 1-127");
    while (Serial.available() == 0) {
     delay(1); // Wait for input from Python
    }
    String idnew = Serial.readStringUntil('\n'); // Read the ID
    idnew.trim(); // Remove any unwanted whitespace
    id = atoi(idnew.c_str());
    if (id == 0) {// ID #0 not allowed, try again!
       return;
    }
    lcd.clear();
    lcd.print("ENROLL: "+ id); 
    while (! getFingerprintEnroll() );
    delay(1000);
  }
  else{
    getFingerprintID();
    delay(50);   
  }
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  lcd.clear();
  lcd.print("waiting finger");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("image taken");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Comm error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging error");
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
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("com error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("no finger feat");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("no finger feat");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();
  lcd.print("Put same finger");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("com error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging error");
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
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("comm error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("no finger feat");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("no finger feat");
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
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("com error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print("Prints not matched");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("stored");
    Serial.print("Stored: " + id);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("error com");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.print("can't store");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.print("error");
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }

  return true;
}
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("image taken");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print(".");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("com error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("imaging error");
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
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("comm error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("no finger feat");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("no finger feat");
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
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("com error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.print("no match found");
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }

  // found a match!
  lcd.clear();
  lcd.print("ID: " + finger.fingerID);
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  lcd.clear();
  lcd.print("confidence: " + finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  return finger.fingerID;
}

