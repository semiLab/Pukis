

/*
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 * CE -> 9
 * CSN -> 10
 *
 * Servo -> 6
 * 
 * Motor enable -> 5
 * Motor fwd -> 7
 * Motor rev -> 8
 * PWM pins: 3,5,6,9,10,11
 *
 * Pins free for lights:
 * (0,1) 2, 3(PWM),4
 * fwd, rear, left, right blink, reversing lights
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>



RF24 radio(9,10);
Servo steering;
int steeringPin = 6;


int ena = 5;
int in1 = 7;
int in2 = 8;

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

uint8_t payload[2];

void setup(){
 
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(ena, HIGH);
  analogWrite(in1, 0);
  analogWrite(in2, 0);

  steering.attach(steeringPin);
  steering.write(90); //Center steering
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
   // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(2);  
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
//  radio.setPALevel(RF24_PA_HIGH); //Probably default, but still...
  radio.setDataRate(RF24_250KBPS); //Default should be 1Mb, 250 is more robust.

    //From the scanner: channel 0x50 to 0x7f seem noise-free
  radio.setChannel(0x60);
  
  radio.startListening();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  //Serial.begin(57600); //This will make digital 0 and 1 RX and TX...
}

void loop(){
  byte recv[2];
  byte send[2];
  //Serial.println("looping");
  if(radio.available()){
    //Serial.println("Got data");
    radio.read(&recv, sizeof(recv));
    //Serial.print("Got: ");
    //Serial.println(recv[1]);
    float val;
    int sval;
    switch(recv[0]){
      case 0:
        val = (float)recv[1] / 255.0 * 90.0;
        val += 45.0;
        sval = (int)val;
        steering.write(val);
      break;
      case 1:
        //The throttle is inverted, so...
        if(recv[1] > 120 && recv[1] < 130){ //Stop and brake
          digitalWrite(ena, HIGH);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
        }else if(recv[1] >= 130){ //Reverse
          byte speed = (recv[1] - 130) * 2;
          analogWrite(ena, speed);
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
        }else if(recv[1] <= 120){ //Forward
          byte speed;
          if(recv[1] > 20)
            speed = (120 - recv[1]) * 2;
          else
            speed = 255 - recv[1] * 2;
          analogWrite(ena, speed);
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
        }
      break;
    }
  }
  //delay(500);
}
