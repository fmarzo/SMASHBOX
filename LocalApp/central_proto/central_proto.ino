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

  // set the data rate for the sensor serial port
  finger.begin(57600);

  pinMode(4, INPUT);
  pinMode(10,INPUT); // pulsante
  lcd.begin(16,1);
  lcd.setCursor(0,0);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  
  //finger.emptyDatabase(); //SE VUOI CANCELLARE TUTTI I TEMPLATE REGISTRATI 

  /* Sending a "0" to let the bridge recognize Central */
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

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
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
  ENR=0;
  
  //LEGGO SE IL BRIDGE MANDA IL LOCK 
  if (Serial.available() > 0) 
  {
    read = Serial.read();
    if (read == CHAR_INFR) 
    {
      //UTILIZZO COME MECCANISMO DI SBLOCCO IL SENSORE AD INFRAROSSI ALTRIMENTI NON ESCI DA LOOP
      while(1)
      {
        if (digitalRead(10) == LOW)
        {
          break;
        }
        Serial.print("00000000001"); 
      }
    } 
  }

  if (digitalRead(4) == LOW) 
  {
    ENR = 1;
  } 
  else 
  {
    ENR = 0;
  }

  if (ENR == 1)
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
    while (! getFingerprintEnroll() );
    delay(1000);
  }
  else
  {
    getFingerprintID();
    delay(50);   
  }

  if (check == 0){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("00000000000"); 
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }

}

