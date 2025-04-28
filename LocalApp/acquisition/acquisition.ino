#include <Wire.h>
#include <ADXL345.h>
#include <stdlib.h>
#include <Adafruit_AHTX0.h>
#include "config.h"
#include "sensors.h"

/* GLOBAL VARIABLES */
ADXL345 accel(ADXL345_ALT); 
sensors_event_t temperature, humidity;
Adafruit_AHTX0 aht;
uint8_t safe_mode = 0;
packet_raw_t packet;
int relock = BOX_NOT_OPENED; /* variable used to check if the box has been fisically opened and closed in order to reset the lock variable to 0 */
bool active_infr = false;

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
  digitalWrite(MAGNET_PIN, LOW);


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
  

while (Serial.available() > 0) Serial.read();

#ifdef ASSIGN_ID_FROM_CENTRAL
  uint8_t buffer[10] = {0u};
  while(1)
  {
      if (Serial.available() >= 10) 
      {  
        Serial.readBytes(buffer, 10);
        packet.id = buffer[0];

        clear_buffer();
        /* clear internal buffer after reading */
        //while (Serial.available() > 0) Serial.read();

        break;  
      }
  }
#else
  packet.id = 122u;
#endif

}

void send_packet (packet_raw_t* packet)
{  

  Serial.write((uint8_t*)packet, sizeof(packet_raw_t));

  if (packet->infr > 0)
  {
    active_infr == true;
    packet->infr = 0;
  }
}

void loop() 
{
 #ifndef SIMULATION_MODE

    update_lock_field(&(packet.lock), &safe_mode);

    if (safe_mode == 0u)
    {
      packet.pres = update_pres_field();

      packet.open = update_open_field (&packet.lock, &relock);
  
      if (!active_infr)
      {
        packet.infr = update_accel_field(&accel);
      }
  
      update_temp_field(&aht, &temperature, &humidity, &packet);
    }
    else
    {
      packet.pres = 0u;
      packet.open = 0u;
      packet.infr = 0u;
      packet.humidity = 0u;
      packet.temp = 0u;
    }

    send_packet(&packet);

  #else
    Serial.print(ID + SIMULATION_STRING);

  #endif

  delay(1500);  /* Delay to prevent overly fast readings */
}


