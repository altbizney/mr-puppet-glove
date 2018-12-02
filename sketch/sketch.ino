#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int FLEX_PIN = A0; // Pin connected to flex sensor voltage divider output

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float FLEX_VCC = 4.98; // Measured voltage of Ardunio 5V line
const float FLEX_R_DIV = 47500.0; // Measured resistance of 3.3k resistor



void setup(void)
{
  Serial.begin(250000); // You can choose any baudrate, just need to also change it in Unity.
  while (!Serial); // wait for Leonardo enumeration, others continue immediately

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  // listen to flex pin
  pinMode(FLEX_PIN, INPUT);

  delay(1000);

  bno.setExtCrystalUse(true);
}

void loop(void)
{
  // Read the ADC, and calculate voltage and resistance from it
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * FLEX_VCC / 1023.0;
  float flexR = FLEX_R_DIV * (FLEX_VCC / flexV - 1.0);

  imu::Quaternion quat = bno.getQuat();

  Serial.print(quat.x(), 4);
  Serial.print(",");
  Serial.print(quat.y(), 4);
  Serial.print(",");
  Serial.print(quat.z(), 4);
  Serial.print(",");
  Serial.print(quat.w(), 4);
  Serial.print(",");
  Serial.print(flexR);
  Serial.println("");

  delay(100);
}
