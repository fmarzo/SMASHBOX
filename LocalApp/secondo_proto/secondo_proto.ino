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
double X, Y, Z;
sensors_event_t humidity, temp;
String pres, infr, lock, open;
int read;
String ID;
bool firstRun = true;  // Variabile per verificare la prima esecuzione

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(13, LOW);

  byte deviceID = accel.readDeviceID();
  if (deviceID == 0) {
    Serial.println("X1");
    while (1) {
      delay(100);
    }
  }
  if (!aht.begin()) {
    Serial.println("X2");
    while (1)
      delay(100);
  }

  if (!accel.writeRate(ADXL345_RATE_200HZ)) {
    Serial.println("X3");
    while (1) {
      delay(100);
    }
  }

  if (!accel.writeRange(ADXL345_RANGE_16G)) {
    Serial.println("X4");
    while (1) {
      delay(100);
    }
  }

  if (!accel.start()) {
    Serial.println("X6");
    while (1) {
      delay(100);
    }
  }

  lock = "0"; //di default non è attivo il lock

  //ID = "015";
  char buffer_id [4] = {0};
  while(1)
  {
      if (Serial.available() >= 3) 
      {  // Controlla se ci sono dati in ingresso
        Serial.readBytes(buffer_id, 3);
        ID.concat(buffer_id);
        break;
      }
    //Serial.print("stalloooooo");
    //delay(1000);
  }
}


void loop() {
  // Lettura di 1 byte dalla Serial 


  if (Serial.available() > 0) {
    read = Serial.read();
    if (read == 49) {
      lock = "1";
    } else {
      lock = "0";
    }
    while (Serial.available()) {
      Serial.read();
    } //modo per pulire il buffer
  }

  if (digitalRead(4) == LOW) {
    pres = "1";
  } else {
    pres = "0";
  }
  if (digitalRead(5) == LOW) {
    open = "0";
  } else {
    open = "1";
  }

  aht.getEvent(&humidity, &temp);

  // Verifica se è la prima esecuzione e salta il controllo iniziale
  if (firstRun) {
    firstRun = false;
    accel.update();
    X = accel.getX();
    Y = accel.getY();
    Z = accel.getZ();

  } else {
    // Aggiorna i dati dell'accelerometro
    if (accel.update()) {
      // Versione in cui invio allarme
      if (abs(X - accel.getX()) > 0.5 || abs(Y - accel.getY()) > 0.5 || abs(Z - accel.getZ()) > 0.5) {
        infr = "1";
      } else {
        infr = "0";
      }

      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);

      X = accel.getX();
      Y = accel.getY();
      Z = accel.getZ();
      //Serial.print("\n");
      Serial.print(ID + pres + round(temp.temperature) + round(humidity.relative_humidity) + infr + lock + open);
    } else {
      Serial.println("X6");
      while (1) {
        delay(100);
      }
    }
  }

  delay(2000);  // Ritardo per evitare letture troppo rapide
}
