#include "sensors.h"
#include <stdlib.h>

bool firstRun = true;
double X, Y, Z; 

void error_handler(int error)
{
  Serial.print("Error ");
  Serial.print(error);

  while (1)
  {
    ;
  }
}

uint8_t update_open_field(uint8_t* lock, int* relock)
{
  uint8_t tmp_open = 0;

    if (digitalRead(OPEN_PIN) == LOW) 
    {
      tmp_open = 0;
      if (*relock == BOX_OPENED)
      { 
        /* the box has already been opened (relock == 1) and it has been closed (digitalRead(OPEN_PIN) == LOW), so we set the lock to 1 */
        *lock = 1; 
        digitalWrite(MAGNET_PIN,HIGH); /* reset de magnet on as soon as we close the box */
        *relock = BOX_NOT_OPENED; /* resetting the lock to the default value */
        digitalWrite(LED_PIN, LOW);
      }
    } 
    else
    {
      /* the box can be physically opened only if it has been unlocked throught the sensor (lock == 0) */
      if(*lock == 0)
      { 
          tmp_open = 1;
          *relock = BOX_OPENED; /* since i've opened the box we set the relock variable to 1 */
      }
    }

    return tmp_open;
}

void update_lock_field(uint8_t* lock, uint8_t* safe_mode)
{
  int read = 0;

    if (Serial.available() > 0) 
    {
      read = Serial.read();

      if (read == CHAR_UNLOCK) 
      {
        /* a simple unlock for the customer is required */
        *lock = 0u;
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(MAGNET_PIN,LOW);
      } 
      else if (read == CHAR_SAFE_MODE)
      {
        /* a threshold has been overpassed on the server */
        /* bridge is notifing acquisitions with ALARM CHAR */
        *lock = 1u;
        *safe_mode = 1;
        digitalWrite(MAGNET_PIN,HIGH);
      }
      else if (read == RE_SAFE_CHAR)
      {
        /* a manual operation on central has been performed */
        /* bridge is notifing acquisitions for quit safe mode */
        /* acquisitions still locked for safety */
        active_infr = false;
        *lock = 1u;
        *safe_mode = 0;
        digitalWrite(MAGNET_PIN,HIGH);
      }
      else 
      {
        digitalWrite(MAGNET_PIN,HIGH);
      }

      clear_buffer();
    }
}

uint8_t update_pres_field()
{
    if (digitalRead(PRESENCE_PIN) == LOW) 
    {
      return 1u;
    } 
    else 
    {
      return 0u;
    }
}

uint8_t update_accel_field(ADXL345* accel)
{
  uint8_t infr = 0u;

  /* if it is the first iteration you can't compare the value with the ones of the previous iteration */
    if (firstRun) 
    {
      firstRun = false;
      accel->update();
      X = accel->getX();
      Y = accel->getY();
      Z = accel->getZ();
    } 
    else 
    {
      /* updating accellerometer parameters */

      if (accel->update())
      {
        if (abs(X - accel->getX()) > 0.2 || abs(Y - accel->getY()) > 0.2 || abs(Z - accel->getZ()) > 0.2) 
        {
          infr = 1u;
        } 
        else 
        {
          infr = 0u;
        }

        X = accel->getX();
        Y = accel->getY();
        Z = accel->getZ();

      } 
      else 
      {
        error_handler(ACCEL_ERR_UPDATE);
      }
    }

    return infr;
}

void update_temp_field(Adafruit_AHTX0* aht, sensors_event_t* temperature, sensors_event_t* humidity, packet_raw_t* packet)
{
  aht->getEvent(humidity, temperature);
  packet->temp = (uint8_t) round(temperature->temperature);
  packet->humidity = (uint8_t) round(humidity->relative_humidity);
}
