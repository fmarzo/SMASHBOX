#ifndef _SENSORS
#define _SENSORS

#include <stdlib.h>

typedef struct packet_t
{
  String open;
  String id;
  sensors_event_t humidity;
  sensors_event_t temp;
  String lock;
  String infr;
  String pres;
}packet_t;

void error_handler(int error);
String update_open_field(String* lock, int* relock);
String update_lock_field();
String update_pres_field();
String update_accel_field(ADXL345* accel);
void update_temp_field(Adafruit_AHTX0* aht, packet_t* packet);

#endif