#ifndef _SENSORS
#define _SENSORS

#include <stdlib.h>

typedef struct __attribute__((packed)) packet_raw_t
  {
    uint8_t id;
    uint8_t pres;
    uint8_t temp;
    uint8_t humidity;
    uint8_t infr;
    uint8_t lock;
    uint8_t open;
  }packet_raw_t;

void error_handler(int error);
uint8_t update_open_field(int* lock, int* relock);
uint8_t update_lock_field();
uint8_t update_pres_field();
uint8_t update_accel_field(ADXL345* accel);
void update_temp_field(Adafruit_AHTX0* aht, sensors_event_t* temperature, sensors_event_t* humidity);

#endif