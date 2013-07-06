#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "Tlc5940.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

/* The TLC5940 has 16 channels (0-15)

   Channels to use:
   

*/

//TODO: Get setup from old code

void setup(){
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();  
  Tlc.init();
}


void loop(){
  
}
