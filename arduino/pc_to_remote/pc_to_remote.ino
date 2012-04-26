#include <Wire.h>
#define TXPIN 0

#define CMD_NONE 0
#define CMD_FWD 1
#define CMD_REV 2
#define CMD_LEFT 3
#define CMD_RIGHT 4

void setup(){
  Wire.begin();
  Serial.begin(9600);
  pinMode(TXPIN, OUTPUT);
}

byte incoming;
byte lastCommand = CMD_NONE;
byte vals[5] = {0,0,0,0,0};
void loop(){
  if(Serial.available() > 0){
    incoming = Serial.read();
    if(lastCommand != 0){
      vals[lastCommand] = incoming;
      lastCommand = 0;
    }else{
      lastCommand = incoming;
    }
  }
}

/*
  Commands:
  4 end code
  10 fwd
  16 fwd+turbo
  22 Turbo
  28 Turbo+fwd+left
  34 Turbo+fwd+right
  40 back
  46 back+right
  52 back+left
  58 Left
  64 Right
  
*/

void sendCmd(int cmd){
  w2(4);
  w1(cmd);
  w2(4);
  w1(cmd);
  w2(4);
  
  w1(4);
  w2(4);
}

void w1(int n){
  for(int i=0;i<n;i++){
    digitalWrite(TXPIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(TXPIN, LOW);
    delayMicroseconds(500);
  }
}

void w2(int n){
 for(int i=0;i<n;i++){
    digitalWrite(TXPIN, HIGH);
    delayMicroseconds(1500);
    digitalWrite(TXPIN, LOW);
    delayMicroseconds(500);
  }
}
