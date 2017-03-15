#include <Wire.h>
#include <gyro.h>

Gyro gyro = Gyro(55);

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  gyro.setup();
}

void loop(void) 
{
  gyro.loop();
  delay(100);
}
