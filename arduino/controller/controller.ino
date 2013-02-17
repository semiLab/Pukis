//Mep
/*
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 * CE -> 9
 * CSN -> 10
 *
 * 
 *
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"



RF24 radio(9,10);


const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

uint8_t payload[2];

void setup(){
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
   // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(2);  
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
 // radio.setPALevel(RF24_PA_HIGH); //Probably default, but still...
  radio.setDataRate(RF24_250KBPS); //Default should be 1Mb, 250 is more robust.
  
  //From the scanner: channel 0x50 to 0x7f seem noise-free
  radio.setChannel(0x60);
  
  radio.startListening();
 // Serial.begin(57600);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop(){
  int val1 = 0;
  int val2 = 0;
  byte send[2];
  val1 = 1023 - analogRead(A4);
  val2 = analogRead(A5);
 /* Serial.print("A4: ");
  Serial.print(val1);
  Serial.print(",A5: ");
  Serial.print(val2);
  Serial.println();*/
  send[0] = 0;
  send[1] = val1 >> 2;
  radio.stopListening();
  bool ok = radio.write(&send, sizeof(send));
  send[0] = 1;
  send[1] = val2 >> 2;
  ok = radio.write(&send, sizeof(send));
  //if(ok)
    //Serial.println("Sent!");
  radio.startListening();
  delay(50);
}
