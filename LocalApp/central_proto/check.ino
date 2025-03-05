#include "check.h"

String padLeft(String str, int length, char padChar = ' ') 
{
  while (str.length() < length) 
  {
    str = padChar + str;  /* adding on left the padding character */
  }
  return str;
}

uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();
  switch (p) 
  {
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
      lcd.clear();
      lcd.print("Unknown");
      return p;
  }

  /* OK success! */

  p = finger.image2Tz();
  switch (p) 
  {
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
      lcd.clear();
      lcd.print("Unknown");
      return p;
  }

  /* OK converted! */
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("match found");
    delay(500);
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    lcd.clear();
    lcd.print("com error");
    delay(500);
    return p;
  } 
  else if (p == FINGERPRINT_NOTFOUND) 
  {
    lcd.clear();
    lcd.print("no match found");
    delay(500);
    return p;
  } 
  else 
  {
    lcd.clear();
    lcd.print("Unknown");
    return p;
  }
  /* found a match! */
  lcd.clear();
  lcd.print("ID: " );
  lcd.print(finger.fingerID);
  delay(500);
  Serial.print("01" + padLeft(String(finger.fingerID), 3, '0') + "000000" );
  lcd.clear();
  lcd.print("confidence: " + finger.confidence);
  delay(500);
  // TODO: questa serve? Se no, togliamo. Se si (credo di si), è la stessa di central_proto.ino? Perchè in quel caso va gestita diversamente (ne parliamo a voce)
  check = 1;

  return finger.fingerID;
}