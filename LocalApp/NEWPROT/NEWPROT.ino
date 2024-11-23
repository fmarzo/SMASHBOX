#include <Wire.h>
#include <ADXL345.h>
#include <stdlib.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
ADXL345 accel(ADXL345_ALT); //SDO low
double X,Y,Z;
sensors_event_t humidity, temp;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(4,INPUT);

  byte deviceID = accel.readDeviceID();
  if (deviceID != 0) {
    Serial.print("0x");
    Serial.print(deviceID, HEX);
    Serial.println("");
  } else {
    Serial.println("read device id: failed");
    while(1) {
      delay(100);
    }
  }
  if(!aht.begin()){
    Serial.println("could not find AHT");
    while(1) delay(10);
  }

  // Data Rate
  // - ADXL345_RATE_3200HZ: 3200 Hz
  // - ADXL345_RATE_1600HZ: 1600 Hz
  // - ADXL345_RATE_800HZ:  800 Hz
  // - ADXL345_RATE_400HZ:  400 Hz
  // - ADXL345_RATE_200HZ:  200 Hz
  // - ADXL345_RATE_100HZ:  100 Hz
  // - ADXL345_RATE_50HZ:   50 Hz
  // - ADXL345_RATE_25HZ:   25 Hz
  // - ...
  if (!accel.writeRate(ADXL345_RATE_200HZ)) {
    Serial.println("write rate: failed");
    while(1) {
      delay(100);
    }
  }

  // Data Range
  // - ADXL345_RANGE_2G: +-2 g
  // - ADXL345_RANGE_4G: +-4 g
  // - ADXL345_RANGE_8G: +-8 g
  // - ADXL345_RANGE_16G: +-16 g
  if (!accel.writeRange(ADXL345_RANGE_16G)) {
    Serial.println("write range: failed");
    while(1) {
      delay(100);
    }
  }

  if (!accel.start()) {
    Serial.println("start: failed");
    while(1) {
      delay(100);
    }
  }
  
}

void loop() {
  if(digitalRead(4)==LOW){
    Serial.print("PIENA ");
  }
  else
  {
    Serial.print("VUOTA ");
  }
  aht.getEvent(&humidity, &temp);
  Serial.print("temp:");
  Serial.print(temp.temperature);
  Serial.print(" Humidity:");
  Serial.print(humidity.relative_humidity);
  Serial.print("% ");
  if (accel.update()) {
    Serial.print("X: ");
    Serial.print(accel.getX());
    Serial.print(",");
    Serial.print(" Y: ");
    Serial.print(accel.getY());
    Serial.print(",");
    Serial.print(" Z: ");
    Serial.print(accel.getZ());
    Serial.println("");
    /* Versione in cui invio allarme
    if (abs(X - accel.getX()) > 0.5 || abs(Y - accel.getY()) > 0.5 ||abs(Z - accel.getZ()) > 0.5 ){
      Serial.print(" infraction");
      Serial.println("");
    }
    else{
      Serial.print(" OK");
      Serial.println("");
    }
    X = accel.getX();
    Y = accel.getY();
    Z = accel.getZ();*/

  } else {
    Serial.println("update failed");
    while(1) {
      delay(100);
    }
  }
  delay(2000);
}
