#include "check.h"

uint8_t getFingerprintID()
{
    lcd.clear();

    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
    {
        lcd.print("Waiting finger..");
        return p;
    }
      
    lcd.clear();  
    lcd.print("Image taken..");

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
    {
        return p;
    }

    lcd.clear();  
    lcd.print("Image converted..");

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {
        lcd.clear();
        lcd.println("Match not found");
        return p;
    }

    /* found a match! */
    lcd.clear();
    lcd.print("ID: " );
    lcd.print(finger.fingerID);
    delay(800);
    uint8_t packet_check[ACTION_PACKET_SIZE] = {PACKET_CHECK, finger.fingerID};
    Serial.write(packet_check, ACTION_PACKET_SIZE);
    return p;
}
