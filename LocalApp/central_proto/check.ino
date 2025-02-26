#include "check.h"

String padLeft(String str, int length, char padChar = ' ') {
  while (str.length() < length) {
    str = padChar + str;  // Aggiunge il carattere di padding a sinistra
  }
  return str;
}

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
  lcd.print("ID: " );
  lcd.print(finger.fingerID);
  delay(500);
  Serial.print("01" + padLeft(String(finger.fingerID), 3, '0') + "000000" );
  lcd.clear();
  lcd.print("confidence: " + finger.confidence);
  delay(500);
  check = 1;

  return finger.fingerID;
}