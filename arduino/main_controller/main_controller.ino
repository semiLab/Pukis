
#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available > 0)
  {
  }
}
