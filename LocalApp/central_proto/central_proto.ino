#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include "config.h"
#include "check.h"
#include "enroll.h"

/* GLOBAL VARIABLES */

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id = 0;
int check = 0;
const int rs = 11, en = 12, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t zero_bytes[7] = { 0, 0, 0, 0, 0, 0, 0 };

uint8_t idle_packet[ACTION_PACKET_SIZE] = {PACKET_IDLE,0};
// packet check is in check.ino 
// packet enroll is in enroll.ino 
uint8_t safe_packet[ACTION_PACKET_SIZE] = {PACKET_SAFE,0};
uint8_t idle_packet_init[PACKET_SIZE] = {0,0,0,0,0,0,0};

void setup() {
  int init_response = 0;

  Serial.begin(STANDARD_BAUDRATE);
  while (!Serial)
    ;
  delay(100);

  /* set the data rate for the sensor serial port */
  finger.begin(FINGER_SENSOR_DATARATE);

  pinMode(SWITCH_ENROLL_PIN, INPUT);
  pinMode(UNLOCK_BUTTON_PIN, INPUT_PULLUP); /* INPUT_PULLUP Enable the internal arduino pull-up resistor*/
  lcd.begin(16, 1);
  lcd.setCursor(0, 0);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);

  /* In case we would like to erase all register IDs */
  finger.emptyDatabase();

  /* Sending a "0" to let the bridge recognize Central */
  Serial.write(idle_packet_init, sizeof(idle_packet_init));

#ifdef CENTRAL_RECOG
  while (1)
  {
      if (Serial.available() > 0) 
      {
          init_response = Serial.read();
          lcd.print(init_response);

          if (init_response == CENTRAL_RECOGNIZE_CHAR)
          {
            lcd.clear();
            lcd.print("FOUND");
            Serial.write(idle_packet, sizeof(idle_packet));
            break;
          }
      }
      else
      {   
          lcd.clear();
          lcd.print("Wait for Bridge");
          delay(500);
      }
  }
#endif
}

int check_for_existing_id(uint8_t id) {
  if (finger.loadModel(id) == FINGERPRINT_OK) {
    return ID_EXISTING_ERR;
  }
  return NO_ERROR;
}

/* --------------------- LOOP --------------------*/
uint8_t enr = 2;

void loop() {
  check = 0;
  int read = 0;

  /* Read if bridge sends lock */
  // TODO: make it as interrupt?
  if (Serial.available() > 0) 
  {
    read = Serial.read();
    if (read == INFRING_CHAR) 
    {
      while (1) 
      {
        if (digitalRead(UNLOCK_BUTTON_PIN) == LOW) 
        {
          lcd.clear();
          lcd.print("Unlock confirmed!");
          delay(1000);

          Serial.write(safe_packet, sizeof(safe_packet));
          break;
        }

        lcd.clear();
        lcd.print("ALARM!");
        lcd.clear();
        lcd.print("MANUAL OP REQUIRED!");
        delay(500);
      }
    }
  }

  //if (digitalRead(SWITCH_ENROLL_PIN) == LOW)
  //{
  //  enr = 1;
  //}
  //else
  //{
  //  enr = 0;
  //}

  if (enr != 0) 
  {
    lcd.clear();
    lcd.print("ENROLL");
    delay(500);

    id = random(MIN_ID_NUM, MAX_ID_NUM);

    while (check_for_existing_id(id) != NO_ERROR) 
    {
      id = random(MIN_ID_NUM, MAX_ID_NUM);
    }

    lcd.clear();
    lcd.print("ENROLL: " + String(id));
    delay(500);
    while (!getFingerprintEnroll())
      ;
    delay(1000);
    enr--;
  }
  else 
  {
    getFingerprintID();
    delay(50);
  }

  if (check == 0) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.write(idle_packet, sizeof(idle_packet));
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
