#include "enroll.h"

uint8_t getFingerprintEnroll() 
{
  int p = -1;
  lcd.clear();
  lcd.print("Waiting finger..");
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    delay(200);

    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("image taken");
      delay(800);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("Waiting finger..");
      delay(800);
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
      lcd.clear();
      lcd.print("Unknown");
      break;
    }
  }

  /* OK success! */

  p = finger.image2Tz(1);
  switch (p) 
  {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(800);
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
      lcd.clear();
      lcd.print("Unknown");
      return p;
  }
  lcd.clear();
  lcd.print("Remove finger");
  delay(800);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();

  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      delay(800);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.print("Put same finger");
      delay(800);
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
      lcd.clear();
      lcd.print("Unknown");
      break;
    }
  }

  /* OK success! */

  p = finger.image2Tz(2);
  switch (p) 
  {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(800);
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
      lcd.clear();
      lcd.print("Unknown");
      return p;
  }

  /* OK converted! */

  p = finger.createModel();
  if (p == FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("Prints matched");
    delay(500);
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    lcd.clear();
    lcd.print("com error");
    delay(500);
    return p;
  } 
  else if (p == FINGERPRINT_ENROLLMISMATCH) 
  {
    lcd.clear();
    lcd.print("Prints not matched");
    delay(500);
    return p;
  } 
  else 
  {
    lcd.clear();
    lcd.print("Unknown");
    return p;
  }
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("Stored!");
    delay(500);
    uint8_t packet_enroll[ACTION_PACKET_SIZE] = {2, id};
    Serial.write(packet_enroll, ACTION_PACKET_SIZE);
    check = 1;
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
    lcd.clear();
    lcd.print("Unknown");
    return p;
  }

  return true;
}