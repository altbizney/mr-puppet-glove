
#include <Wire.h>
#include <SPI.h>

extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI_anything.h>

Adafruit_BNO055 wrist = Adafruit_BNO055(55);
Adafruit_BNO055 elbow = Adafruit_BNO055(55);
Adafruit_BNO055 shoulder = Adafruit_BNO055(55);

bool wrist_found_direct = false;
bool wrist_found = false;
bool elbow_found = false;
bool shoulder_found = false;

#define TCAADDR 0x70
#define BAUD 115200
#define WRIST_ADDR 2
#define ELBOW_ADDR 1
#define SHOULDER_ADDR 0

typedef struct myStruct
{
  byte a;
  int b;
  long c;
};

myStruct foo;

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void print_sensor_data(Adafruit_BNO055 *bno)
{
  imu::Quaternion quat = bno->getQuat();

  Serial.print(quat.x(), 4);
  Serial.print(",");
  Serial.print(quat.y(), 4);
  Serial.print(",");
  Serial.print(quat.z(), 4);
  Serial.print(",");
  Serial.print(quat.w(), 4);

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

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  foo.a = 42;
  foo.c = 100000;

  Serial.begin(BAUD);

  // count how many devices are connected
  byte device_count = 0;
  bool found_tca = false;

  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print("DEBUG: Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);

      if (i == TCAADDR) {
        found_tca = true;
        Serial.print(") (**TCA MATCH**");
      }

      Serial.println(")");

      device_count++;
      delay (1);  // maybe unneeded?
    }
  }

  Serial.print("DEBUG: Devices found: ");
  Serial.println(device_count);

  if (found_tca) {
    // found a TCA, start sensor scan
    Serial.print("DEBUG:Scanning for wrist on TCA Port #");
    Serial.println(WRIST_ADDR);
    tcaselect(WRIST_ADDR);

    for (uint8_t addr = 0; addr <= 127; addr++) {
      if (addr == TCAADDR) continue;

      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
        Serial.print("DEBUG:Found wrist at I2C 0x");
        Serial.println(addr, HEX);
        wrist_found = true;
      }
    }

    Serial.print("DEBUG:Scanning for elbow on TCA Port #");
    Serial.println(ELBOW_ADDR);
    tcaselect(ELBOW_ADDR);

    for (uint8_t addr = 0; addr <= 127; addr++) {
      if (addr == TCAADDR) continue;

      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
        Serial.print("DEBUG:Found elbow at I2C 0x");
        Serial.println(addr, HEX);
        elbow_found = true;
      }
    }

    Serial.print("DEBUG:Scanning for shoulder on TCA Port #");
    Serial.println(SHOULDER_ADDR);
    tcaselect(SHOULDER_ADDR);

    for (uint8_t addr = 0; addr <= 127; addr++) {
      if (addr == TCAADDR) continue;

      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
        Serial.print("DEBUG:Found shoulder at I2C 0x");
        Serial.println(addr, HEX);
        shoulder_found = true;
      }
    }

    // init wrist
    if (wrist_found) {
      tcaselect(WRIST_ADDR);
      if (!wrist.begin())
      {
        Serial.println("DEBUG: wrist was found but is not reachable");
        while (1);
      }
      delay(1000);
      wrist.setExtCrystalUse(true);
      Serial.println("DEBUG: wrist setup complete");
    } else {
      Serial.println("DEBUG: wrist not found, skipping");
    }

    // init elbow
    if (elbow_found) {
      tcaselect(ELBOW_ADDR);
      if (!elbow.begin())
      {
        Serial.println("DEBUG: elbow was found but is not reachable");
        while (1);
      }
      delay(1000);
      elbow.setExtCrystalUse(true);
      Serial.println("DEBUG: elbow setup complete");
    } else {
      Serial.println("DEBUG: elbow not found, skipping");
    }

    // init shoulder
    if (shoulder_found) {
      tcaselect(SHOULDER_ADDR);
      if (!shoulder.begin())
      {
        Serial.println("DEBUG: shoulder was found but is not reachable");
        while (1);
      }
      delay(1000);
      shoulder.setExtCrystalUse(true);
      Serial.println("DEBUG: shoulder setup complete");
    } else {
      Serial.println("DEBUG: shoulder not found, skipping");
    }
  } else {
    // no TCA found. try direct access to the wrist sensor
    Serial.println("DEBUG: trying direct access to wrist");

    if (!wrist.begin()) {
      Serial.println("DEBUG: wrist not directly connected, skipping");
    } else {
      delay(1000);
      wrist.setExtCrystalUse(true);
      wrist_found_direct = true;
      Serial.println("DEBUG: wrist direct access setup complete");
    }
  }
}

void loop() {
  // read hand
  foo.b = analogRead(A0);
  Serial.print(foo.b);
  Serial.print(";");

  // read wrist
  if (wrist_found || wrist_found_direct) {
    if (!wrist_found_direct) tcaselect(WRIST_ADDR);
    print_sensor_data(&wrist);
  } else {
    Serial.print("0,0,0,0,3,3,3,3");
  }
  Serial.print(";");

  // read elbow
  if (elbow_found) {
    tcaselect(ELBOW_ADDR);
    print_sensor_data(&elbow);
  } else {
    Serial.print("0,0,0,0,3,3,3,3");
  }
  Serial.print(";");

  // read shoulder
  if (shoulder_found) {
    tcaselect(SHOULDER_ADDR);
    print_sensor_data(&shoulder);
  } else {
    Serial.print("0,0,0,0,3,3,3,3");
  }

  Serial.println();

  digitalWrite(SS, LOW); // enable Slave Select
  SPI_writeAnything(foo);
  digitalWrite(SS, HIGH); // disable Slave Select

  foo.c++;

  delay(60);
}
