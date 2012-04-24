#include <TinyWireS.h>
/*
Pin config:
1 Front lights
2 Rear Lights
3 Left blinker
4 GND
5 I2C SDA
6 I2C SCK
7 Right blinker
8 Vcc
*/

#define FRONT_LED 11
#define REAR_LED 3
#define LEFT_LED 10
#define RIGHT_LED 5

#define I2C_SLAVE_ADDR 0x42


// Light codes
// command is a byte. Bitmask:
// xxxxxxx1 Lights
// xxxxxx1x left blinker
// xxxxx1xx right blinker
// xxxx1xxx high beams
// xxx1xxxx brake lights

#define LIGHTS 1
#define LEFT 2
#define RIGHT 4
#define HIGH_BEAMS 8
#define BRAKE 16
#define UNUSED1 32
#define UNUSED2 64
#define UNUSED3 128



byte state = 0; //All is off to begin with
byte frontLightIntensity = 0;
byte rearLightIntensity = 0;
boolean turningOnLights = true;
byte blinkerIntensity = 0;
boolean turningOnBlinker = true;
int blinkerSleepLeft = 0;
int blinkerOnLeft = 255;

void setup()
{
  pinMode(FRONT_LED,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(REAR_LED,OUTPUT);
  pinMode(RIGHT_LED,OUTPUT);
  pinMode(LEFT_LED,OUTPUT);
  pinMode(13, OUTPUT);
  TinyWireS.begin(I2C_SLAVE_ADDR);
  state = state | LIGHTS;
}

void loop()
{
  byte recvd = 0;
  if(TinyWireS.available()) // I2C input
  {
    recvd = TinyWireS.receive();
    updateState(recvd);
  }
  makeTheWorldABetterPlace(); 
   //analogWrite(11, 255);
  // analogWrite(FRONT_LED, 255);
  //digitalWrite(13, HIGH);
  //delay(10);
}
void updateState(byte recvd)
{
  state = recvd;
}
void makeTheWorldABetterPlace()
{
  if(state & LIGHTS)
  {
    if(turningOnLights && (frontLightIntensity < 50))
      frontLightIntensity++;
    if(turningOnLights && rearLightIntensity < 100)
      rearLightIntensity++;      
  }
  else
  {

  }
  if(state & (LEFT | RIGHT))
  {
    if(blinkerSleepLeft = 0)
    {
      
      if(blinkerIntensity = 255)
        turningOnBlinker = false;
      blinkerIntensity += (turningOnBlinker? 1 : -1);
      if(blinkerIntensity == 0)
        blinkerSleepLeft = 255; 
    }
    else
      blinkerSleepLeft--;
  }
  else
  {
    blinkerSleepLeft = 0;
  }
  analogWrite(FRONT_LED, frontLightIntensity);
  analogWrite(REAR_LED, rearLightIntensity);
  if(state & LEFT)
    analogWrite(LEFT_LED, blinkerIntensity);
  if(state & RIGHT)
    analogWrite(RIGHT_LED, blinkerIntensity);
  delay(1);
}
