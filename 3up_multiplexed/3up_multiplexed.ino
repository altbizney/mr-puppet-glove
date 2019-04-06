
#include <Wire.h>
extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 wrist = Adafruit_BNO055(55);
Adafruit_BNO055 elbow = Adafruit_BNO055(55);
Adafruit_BNO055 shoulder = Adafruit_BNO055(55);

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void print_sensor_data(Adafruit_BNO055 *bno)
{
  imu::Quaternion quat = bno->getQuat();

  // convert to unity-style quaternions
  Serial.print(quat.w() * -1, 4);
  Serial.print(",");
  Serial.print(quat.y() * -1, 4);
  Serial.print(",");
  Serial.print(quat.z() * -1, 4);
  Serial.print(",");
  Serial.print(quat.x(), 4);

  uint8_t system, gyro, accel, mag = 0;
  bno->getCalibration(&system, &gyro, &accel, &mag);
  Serial.print(",");
  Serial.print(system, DEC);
  Serial.print(",");
  Serial.print(gyro, DEC);
  Serial.print(",");
  Serial.print(accel, DEC);
  Serial.print(",");
  Serial.print(mag, DEC);
}

void setup() {
  while (!Serial);
  delay(1000);

  Wire.begin();

  Serial.begin(250000);

  // scan i2c ports
  Serial.println("DEBUG:TCAScanner ready");
  for (uint8_t t = 0; t < 8; t++) {
    tcaselect(t);
    Serial.print("DEBUG:TCA Port #");
    Serial.println(t);

    for (uint8_t addr = 0; addr <= 127; addr++) {
      if (addr == TCAADDR) continue;

      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
        Serial.print("DEBUG:Found I2C 0x");
        Serial.println(addr, HEX);
      }
    }
  }

  // init wrist on port 0
  tcaselect(0);
  if (!wrist.begin())
  {
    Serial.println("DEBUG: wrist BNO055 not detected");
    while (1);
  }
  delay(1000);
  wrist.setExtCrystalUse(true);
  Serial.println("DEBUG: wrist setup complete");

  // init elbow on port 1
  tcaselect(1);
  if (!elbow.begin())
  {
    Serial.println("DEBUG: elbow BNO055 not detected");
    while (1);
  }
  delay(1000);
  elbow.setExtCrystalUse(true);
  Serial.println("DEBUG: elbow setup complete");

  // init shoulder on port 2
  tcaselect(2);
  if (!shoulder.begin())
  {
    Serial.println("DEBUG: shoulder BNO055 not detected");
    while (1);
  }
  delay(1000);
  shoulder.setExtCrystalUse(true);
  Serial.println("DEBUG: shoulder setup complete");

  Serial.println("DEBUG: setup complete");
}

void loop() {
  // read hand
  Serial.print(analogRead(A0));
  Serial.print("\t");

  // read wrist
  tcaselect(0);
  print_sensor_data(&wrist);
  Serial.print("\t");

  // read elbow
  tcaselect(1);
  print_sensor_data(&elbow);
  Serial.print("\t");

  // read shoulder
  tcaselect(2);
  print_sensor_data(&shoulder);
  Serial.println("");

  delay(60);
}
