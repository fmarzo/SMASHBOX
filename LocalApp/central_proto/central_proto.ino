#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include "config.h"
#include "check.h"
#include "enroll.h"

void setup()
{
  Serial.begin(STANDARD_BOUNDRATE);
  while (!Serial);  
  delay(100);

  /* set the data rate for the sensor serial port */
  finger.begin(FINGERSENSOR_DATARATE);

  pinMode(SWITCHENROLL_PIN, INPUT);
  pinMode(UNLOCKBUTTON_PIN,INPUT);
  lcd.begin(16,1);
  lcd.setCursor(0,0);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  
  /* In case we would like to erase all register IDs */
  // finger.emptyDatabase(); 

  /* Sending a "0" to let the bridge recognize Central */
  Serial.print(NOT_STRING); 

  while (1)
    {
        if (Serial.available() > 0) 
        {
            init_response = Serial.read();
            lcd.print(init_response);
            if (init_response == CENTRAL_CHAR_REGOGNIZE)
            {
              
              lcd.clear();
              lcd.print("FOUND");
              break;
            }
        }
    }
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
  check = 0;
  enr=0;
  
  /* Read if bridge sends lock */
  if (Serial.available() > 0) 
  {
    read = Serial.read();
    if (read == CHAR_INFR) 
    {
      /* Using infrared mechanism, otherwise not quitting from main loop */
      while(1)
      {
        if (digitalRead(UNLOCKBUTTON_PIN) == LOW)
        {
          break;
        }
        Serial.print(INFR_STRING); 
      }
    } 
  }
  if (digitalRead(SWITCHENROLL_PIN) == LOW) 
  {
    enr = 1;
  } 
  else 
  {
    enr = 0;
  }

  if (enr == 1)
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
    while (!getFingerprintEnroll());
    delay(1000);
  }
  else
  {
    getFingerprintID();
    delay(50);   
  }

  if (check == 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(NOT_STRING); 
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }

}

