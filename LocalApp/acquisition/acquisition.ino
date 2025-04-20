#include <Wire.h>
#include <ADXL345.h>
#include <stdlib.h>
#include <Adafruit_AHTX0.h>
#include "config.h"
#include "sensors.h"

/* GLOBAL VARIABLES */
ADXL345 accel(ADXL345_ALT); 
Adafruit_AHTX0 aht;
String lock = "1";
packet_t packet;
int relock = BOX_NOT_OPENED; /* variable used to check if the box has been fisically opened and closed in order to reset the lock variable to 0 */

void setup() 
{
  char buffer_id [4] = {0};
  Serial.begin(STANDARD_BAUDRATE);
  Wire.begin();

  pinMode(PRESENCE_PIN, INPUT);
  pinMode(OPEN_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MAGNET_PIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  byte deviceID = accel.readDeviceID();

  if (deviceID == 0 && 
    !aht.begin() && 
    !accel.writeRate(ADXL345_RATE_200HZ) && 
    !accel.writeRange(ADXL345_RANGE_16G) && 
    !accel.start()) 
  {
    error_handler(INIT_ERR);
  }

  if (deviceID == 0) 
  {
    error_handler(DEVICE_ID_ERR);
  }

  if (!aht.begin()) 
  {
    error_handler(AHT_ERR);
  }
  
  if (!accel.writeRate(ADXL345_RATE_200HZ)) 
  {
    error_handler(ACCEL_ERR_RATE);
  }

  if (!accel.writeRange(ADXL345_RANGE_16G)) 
  {
    error_handler(ACCEL_ERR_RANGE);
  }

  if (!accel.start()) 
  {
    error_handler(ACCEL_ERR_START);
  }
  
#ifdef ASSIGN_ID_FROM_CENTRAL
  while(1)
  {
      if (Serial.available() >= 3) 
      {  
        Serial.readBytes(buffer_id, 3);
        packet->id.concat(buffer_id);
        break;
      }
  }
#else
  packet.id = "122";
#endif

}

void send_packet (packet_t* packet)
{
  Serial.print(packet->id + packet->pres + round(packet->temp.temperature) + round(packet->humidity.relative_humidity) + packet->infr + packet->lock + packet->open);
}

void loop() 
{
 #ifndef SIMULATION_MODE

    packet.lock = update_lock_field();

    packet.pres = update_pres_field();

    packet.open = update_open_field (&lock, &relock);

    packet.infr = update_accel_field(&accel);

    update_temp_field(&aht, &packet);

    send_packet(&packet);

  #else
    Serial.print(ID + SIMULATION_STRING);

  #endif

  delay(100);  /* Delay to prevent overly fast readings */
}


