#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include "config.h"
#include "check.h"
#include "enroll.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  /* set the data rate for the sensor serial port */
  finger.begin(57600);

  // TODO: anche qui, se hai definito le macro in config, puoi sostituire
  pinMode(4, INPUT);
  pinMode(10,INPUT); /* pulsante */
  lcd.begin(16,1);
  lcd.setCursor(0,0);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  
  /* In case we would like to erase all register IDs*/
  /*finger.emptyDatabase();*/ 

  /* Sending a "0" to let the bridge recognize Central */
  // TODO: la stringa "00000000000" la mandiamo più volte. Forse possiamo mettere anche questa in config.h (se non l'hai già fatto)
  Serial.print("00000000000"); 

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

//TODO: è utilizzata questa funzione? Se no, allora togliamola
uint8_t readnumber(void) 
{
  uint8_t num = 0;

  while (num == 0) 
  {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
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
        // TODO: sostituzione macro?
        if (digitalRead(10) == LOW)
        {
          break;
        }
        Serial.print("00000000001"); 
      }
    } 
  }
  // TODO: qui se hai definito in config.h la macro, puoi sostituire digitalRead(4) proprio con il valore del #define
  if (digitalRead(4) == LOW) 
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
    // TODO: enrOLL è voluto? Oppure era Enroll/ENROLL?
    lcd.print("enrOLL");
    delay(500);

    id = random(1,127);

    while (check_for_existing_id (id) != NO_ERROR)
    {
      id = random(1,127);
    }

    lcd.clear();
    // TODO: enrOLL è voluto? Oppure era Enroll/ENROLL?
    lcd.print("enrOLL: "+ String(id)); 
    delay(500);
    while (!getFingerprintenroll());
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
    Serial.print("00000000000"); 
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }

}

