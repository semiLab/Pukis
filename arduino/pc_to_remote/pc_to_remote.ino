#include <Wire.h>
#define TXPIN 0


void setup(){
  Wire.begin();
  Serial.begin(9600);
  pinMode(TXPIN, OUTPUT);
  pinMode(13,OUTPUT);
}
int x, y;
byte incoming;
byte cmd[] = {
  0,0};
byte foo = 0;

void loop(){
  if(Serial.available() > 0){
    incoming = Serial.read();
    if(cmd[0] == 't'){
      if(cmd[1] == 'x'){
        x = incoming - 127;  
      } 
      else if(cmd[1] == 'y'){
        y = incoming;
        Serial.print("y:");
        Serial.println(y);
      }
    }
    cmd[0] = cmd[1];
    cmd[1] = incoming; 
  }
  //analogWrite(13, y);
  digitalWrite(13,HIGH);
  delayMicroseconds(y);
  digitalWrite(13,LOW);
  delayMicroseconds(128-y);
  //  sendCmd(10);
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

