#include "check.h"

uint8_t getFingerprintID()
{
    lcd.clear();
    //lcd.print("Waiting finger..");
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
    {
        return p;
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
    {
        return p;
    }

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {
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
