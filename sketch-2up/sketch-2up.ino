#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, BNO055_ADDRESS_B);

void setup(void)
{
  Serial.begin(250000);
  while (!Serial);

  if (!bno.begin())
  {
    Serial.println("DEBUG: BNO055 1 not detected");
    while (1);
  }

  bno.setExtCrystalUse(true);

  if (!bno2.begin())
  {
    Serial.println("DEBUG: BNO055 2 not detected");
    while (1);
  }

  delay(1000);

  bno2.setExtCrystalUse(true);

  Serial.println("DEBUG: Setup complete");
}

void loop(void)
{
  imu::Quaternion quat = bno.getQuat();
  Serial.print(quat.x(), 4);
  Serial.print(",");
  Serial.print(quat.y(), 4);
  Serial.print(",");
  Serial.print(quat.z(), 4);
  Serial.print(",");
  Serial.print(quat.w(), 4);

  Serial.print(",");
  imu::Quaternion quat2 = bno2.getQuat();
  Serial.print(quat2.x(), 4);
  Serial.print(",");
  Serial.print(quat2.y(), 4);
  Serial.print(",");
  Serial.print(quat2.z(), 4);
  Serial.print(",");
  Serial.print(quat2.w(), 4);

  Serial.print(",");
  Serial.print(analogRead(A0));

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  uint8_t system2, gyro2, accel2, mag2 = 0;
  bno2.getCalibration(&system2, &gyro2, &accel2, &mag2);

  Serial.print(",S1:");
  Serial.print(system, DEC);
  Serial.print(",S2:");
  Serial.print(system2, DEC);
  Serial.print(";G1:");
  Serial.print(gyro, DEC);
  Serial.print(";G2:");
  Serial.print(gyro2, DEC);
  Serial.print(";A1:");
  Serial.print(accel, DEC);
  Serial.print(";A2:");
  Serial.print(accel2, DEC);
  Serial.print(";M1:");
  Serial.print(mag, DEC);
  Serial.print(";M2:");
  Serial.print(mag2, DEC);

  Serial.println("");

  delay(60);
}
