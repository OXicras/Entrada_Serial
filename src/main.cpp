#include <Arduino.h>

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(2000);
  //* readStringUntil espera no máximo esse tempo(ms) pelo '\n'
}

void loop() 
{
  
}