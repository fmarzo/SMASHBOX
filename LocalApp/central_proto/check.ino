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
  delay(500);
  switch (p) 
  {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("image taken");
      delay(800);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("Waiting finger");
      delay(800);
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
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("match found");
    delay(800);
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
    delay(800);
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
  delay(800);
  uint8_t packet_check[ACTION_PACKET_SIZE] = {PACKET_CHECK, finger.fingerID};
  Serial.write(packet_check, ACTION_PACKET_SIZE);
  
  check = 1;

  return finger.fingerID;
}