#include <Wire.h>
#include <ADXL345.h>
#include <stdlib.h>
#include <Adafruit_AHTX0.h>
#include "config.h"

/* GLOBAL VARIABLES */

Adafruit_AHTX0 aht;
ADXL345 accel(ADXL345_ALT); 
double X, Y, Z;
sensors_event_t humidity, temp;
String lock = "1";
String ID;
bool firstRun = true; 
int relock = BOX_NOT_OPENED; /* variable used to check if the box has been fisically opened and closed in order to reset the lock variable to 0 */

void setup() {

  char buffer_id [4] = {0};
  Serial.begin(STANDARD_BOUNDRATE);
  Wire.begin();
  pinMode(PRESENCE_PIN, INPUT);
  pinMode(OPEN_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  byte deviceID = accel.readDeviceID();
  if (deviceID == 0) 
  {
    Serial.print("X1");
    while (1) 
    {
      delay(100);
    }
  }
  if (!aht.begin()) 
  {
    Serial.print("X2");
    while (1)
      delay(100);
  }

  if (!accel.writeRate(ADXL345_RATE_200HZ)) 
  {
    Serial.print("X3");
    while (1) 
    {
      delay(100);
    }
  }

  if (!accel.writeRange(ADXL345_RANGE_16G)) 
  {
    Serial.print("X4");
    while (1) 
    {
      delay(100);
    }
  }

  if (!accel.start()) 
  {
    Serial.print("X6");
    while (1) 
    {
      delay(100);
    }
  }
  
  while(1)
  {
      if (Serial.available() >= 3) 
      {  
        Serial.readBytes(buffer_id, 3);
        ID.concat(buffer_id);
        break;
      }
  }
}


void loop() {
  String open = "0";
  int read;
  String infr, pres;

  if (Serial.available() > 0) 
  {
    read = Serial.read();
    if (read == OPEN_CHAR) 
    {
      lock = "0";
    } 
    else if (read == INFR_CHAR)
    {
      lock = "1";
    }
    else 
    {
      lock = "1";
    }
  }

  if (digitalRead(PRESENCE_PIN) == LOW) 
  {
    pres = "1";
  } 
  else 
  {
    pres = "0";
  }
  if (digitalRead(OPEN_PIN) == LOW) 
  {
    open = "0";
    if (relock == BOX_OPENED)
    { 
      /* the box has already been opened (relock == 1) and it has been closed (digitalRead(OPEN_PIN) == LOW), so we set the lock to 1 */
      lock = "1"; 
      relock = BOX_NOT_OPENED; /* resetting the lock to the deafault value */
    }
  } 
  else
  {
    /* the box can be fisical opened only if it has been unlocked throught the sensor (lock == 0) */
    if(lock == "0")
    { 
        open = "1";
        relock = BOX_OPENED; /* since i've opened the box we set the relock variable to 1 */
    }
  }

  aht.getEvent(&humidity, &temp);

  /* if it is the first iteration you can't compare the value with the ones of the previous iteration */
  if (firstRun) 
  {
    firstRun = false;
    accel.update();
    X = accel.getX();
    Y = accel.getY();
    Z = accel.getZ();
  } 
  else 
  {
    /* updating accellerometer parameters */
    if (accel.update())
    {
      if (abs(X - accel.getX()) > 0.5 || abs(Y - accel.getY()) > 0.5 || abs(Z - accel.getZ()) > 0.5) 
      {
        infr = "1";
      } 
      else 
      {
        infr = "0";
      }

      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);

      X = accel.getX();
      Y = accel.getY();
      Z = accel.getZ();
      Serial.print(ID + pres + round(temp.temperature) + round(humidity.relative_humidity) + infr + lock + open);
    } 
    else 
    {
      Serial.print("X6");
      while (1) 
      {
        delay(100);
      }
    }
  }

  delay(2000);  /* Delay to prevent overly fast readings */
}


