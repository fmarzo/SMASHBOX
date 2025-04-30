#include "enroll.h"

bool enrollFingerprint(int id)
{
    int p = -1;
    lcd.clear();
    
    //String waiting_msg = "Waiting enroll.."
    lcd.print("Waiting enroll...");
    lcd.print(id);
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        if (p == FINGERPRINT_NOFINGER)
        {
            continue;
        }

        if (p != FINGERPRINT_OK)
        {
            lcd.print("Error reading image.");
            return false;
        }
    }

    p = finger.image2Tz(1);
    lcd.clear();
    if (p != FINGERPRINT_OK)
    {
        lcd.print("Error converting image.");
        return false;
    }

    lcd.print("Remove finger");
    delay(2000);
    while (finger.getImage() != FINGERPRINT_NOFINGER);

    lcd.clear();
    lcd.print("Repeat finger...");
    p = -1;
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        if (p == FINGERPRINT_NOFINGER)
        {
            continue;
        }

        if (p != FINGERPRINT_OK)
        {
            lcd.print("Error during second scan.");
            return false;
        }
    }

    p = finger.image2Tz(2);
    lcd.clear();
    if (p != FINGERPRINT_OK)
    {
        lcd.print("Error converting second image.");
        return false;
    }

    p = finger.createModel();
    lcd.clear();
    if (p != FINGERPRINT_OK)
    {
        lcd.print("Print unmatched!");
        return false;
    }

    lcd.clear();
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
    {
        lcd.print("Enrolled!");
        while (finger.getImage() != FINGERPRINT_NOFINGER);

        delay(500);
        uint8_t packet_enroll[ACTION_PACKET_SIZE] = {PACKET_ENROLL, id};
        Serial.write(packet_enroll, sizeof(packet_enroll));
        check = 1;
        return true;
    }
    else
    {
        lcd.print("Unknown");
        return false;
    }
}
