#include <Wire.h>
#include <ADXL345.h>
#include <stdlib.h>
#include <Adafruit_AHTX0.h>
#include "config.h"

void setup() {
  // TODO: anche questa è una costante di configurazione. Possiamo settarla
  Serial.begin(9600);
  Wire.begin();
  // TODO: sostituzione macro?
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(13, LOW);

  byte deviceID = accel.readDeviceID();
  if (deviceID == 0) 
  {
    Serial.println("X1");
    while (1) 
    {
      delay(100);
    }
  }
  if (!aht.begin()) 
  {
    Serial.println("X2");
    while (1)
      delay(100);
  }

  if (!accel.writeRate(ADXL345_RATE_200HZ)) 
  {
    Serial.println("X3");
    while (1) 
    {
      delay(100);
    }
  }

  if (!accel.writeRange(ADXL345_RANGE_16G)) 
  {
    Serial.println("X4");
    while (1) 
    {
      delay(100);
    }
  }

  if (!accel.start()) 
  {
    Serial.println("X6");
    while (1) 
    {
      delay(100);
    }
  }
  
  char buffer_id [4] = {0};
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
  // Lettura di 1 byte dalla Serial 
  int relock = 0; //variabile che serve per rimettere il lock a 0 (chiuso) una volta che la cassetta è stata aperta e poi ri-chiusa
  int read;
  String pres;

  if (Serial.available() > 0) 
  {
    read = Serial.read();
    if (read == OPEN_CHAR) 
    {
      lock = "1";
    } 
    else if (read == INFR_CHAR)
    {
      lock = "0";
    }
    else 
    {
      lock = "0";
    }
  }

  if (PRESENCE == LOW) 
  {
    pres = "1";
  } 
  else 
  {
    pres = "0";
  }
  if (OPEN == LOW) 
  {
    open = "0";
    if (relock == 1)
    { //ovvero la cassetta è già stata aperta ed è stata chiusa, imposta il lock a 0
      lock = "0"; //imposto il lock nuovamente a 0 siccome è già stata aperta e chiusa
      relock = 0; //ri imposto la variabile ausiliaria relock a 0
    }
  } 
  else
  {
    if(lock == "1")//la cassetta si può aprire solo se è stata sbloccata con il lock
    { 
        open = "1";
        relock = 1; //ho aperto la cassetta, imposto la variabile ausiliaria RELOCK a 1
    }
  }

  aht.getEvent(&humidity, &temp);

  // Verifica se è la prima esecuzione e salta il controllo iniziale
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
    // Aggiorna i dati dell'accelerometro
    if (accel.update())
    {
      // Versione in cui invio allarme
      if (abs(X - accel.getX()) > 0.5 || abs(Y - accel.getY()) > 0.5 || abs(Z - accel.getZ()) > 0.5) 
      {
        infr = "1";
      } 
      else 
      {
        infr = "0";
      }

      // TODO: sostizione macro
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);

      X = accel.getX();
      Y = accel.getY();
      Z = accel.getZ();
      // TODO: togliamo se non serve
      //Serial.print("\n");
      Serial.print(ID + pres + round(temp.temperature) + round(humidity.relative_humidity) + infr + lock + open);
    } 
    else 
    {
      Serial.println("X6");
      while (1) 
      {
        delay(100);
      }
    }
  }

  delay(2000);  // Ritardo per evitare letture troppo rapide
}



// NOTE PER IL REFACTORING:

/* 

1. Decidiamo se scrivere tutto in italiano o in inglese (decidi tu, ma meglio in inglese perchè se carichiamo il progetto su Git abbiamo più audiance)
2. Scegliamo uno stile per i commenti. Tendenzialmente io userei / *  * / ma va bene anche //. Però decidiamone uno.
3. Se riusciamo teniamo al massimo 1 riga vuota tra una riga di codice e l'altra.abort
4. Ti ho messo apposto alcune parentesi che ti eri scordato
5. Nota sulla definizione delle macro. In realtà ora che ci penso non andrei a modificare direttamente la funzione (ti mando un audio)
6. Cancelliamo i commenti che non ci aiutano o che non servono più
7. Cancella anche questa sezione una volta che hai finito
*/
