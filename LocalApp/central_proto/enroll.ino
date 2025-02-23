#include "enroll.h"

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
  if (p == FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("Stored!");
    delay(500);
    Serial.print("02" + padLeft(String(id), 3, '0') + "000000" );
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    lcd.clear();
    lcd.print("error com");
    delay(500);
    return p;
  } 
  else if (p == FINGERPRINT_BADLOCATION)
  {
    lcd.clear();
    lcd.print("can't store");
    delay(500);
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) 
  {
    lcd.clear();
    lcd.print("error");
    delay(500);
    return p;
  }
  else 
  {
   // Serial.println("Unknown error");
    return p;
  }

  return true;
}