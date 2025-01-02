String ID, pres, temp, hum, infr, lock, open;
int rx_check = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);           // set pin to input
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  ID = "0";
  pres = "0";
  temp = "10";
  hum = "12";
  infr = "1";
  lock = "0";
  open = "1";

  Serial.print(ID + pres + temp + hum + infr + lock + open);

delay(1000);
 

}
