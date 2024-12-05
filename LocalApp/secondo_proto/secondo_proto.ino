#include <Wire.h>
#include <ADXL345.h>
#include <stdlib.h>
#include <Adafruit_AHTX0.h>
/*
ERROR LEGEND
X1: read device id: failed
X2: could not find AHT
X3: write rate: failed
X4: write range: failed
X5: start: failed
X6: update failed
*/
Adafruit_AHTX0 aht;
ADXL345 accel(ADXL345_ALT); //SDO low
double X,Y,Z;
sensors_event_t humidity, temp;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(4,INPUT);

  byte deviceID = accel.readDeviceID();
  if(deviceID == 0) {
    Serial.println("X1");
    while(1) {
      delay(100);
    }
  }
  if(!aht.begin()){
    Serial.println("X2");
    while(1) delay(100);
  }

  if (!accel.writeRate(ADXL345_RATE_200HZ)) {
    Serial.println("X3");
    while(1) {
      delay(100);
    }
  }

  if (!accel.writeRange(ADXL345_RANGE_16G)) {
    Serial.println("X4");
    while(1) {
      delay(100);
    }
  }

  if (!accel.start()) {
    Serial.println("X6");
    while(1) {
      delay(100);
    }
  }
  
}

void loop() {
  if(digitalRead(4)==LOW){
    Serial.print("1");
  }
  else
  {
    Serial.print("0");
  }
  aht.getEvent(&humidity, &temp);
  Serial.print(round(temp.temperature));
  Serial.print(round(humidity.relative_humidity));
  if (accel.update()) {
    // Versione in cui invio allarme
    if (abs(X - accel.getX()) > 0.5 || abs(Y - accel.getY()) > 0.5 ||abs(Z - accel.getZ()) > 0.5 ){
      Serial.print("1");
    }
    else{
      Serial.print("0");
    }
    X = accel.getX();
    Y = accel.getY();
    Z = accel.getZ();
  } else {
    Serial.println("X6");
    while(1) {
      delay(100);
    }
  }
  delay(2000);
}
