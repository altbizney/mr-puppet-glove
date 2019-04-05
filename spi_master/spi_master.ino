#include <SPI.h>

void setup (void)
{
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  Serial.begin(250000);
  while (!Serial);

  if (!bno.begin())
  {
    Serial.println("DEBUG: BNO055 not detected");
    while (1);
  }

  bno.setExtCrystalUse(true);

  Serial.println("DEBUG: Setup complete");
}

void loop (void)
{
  // enable Slave Select
  digitalWrite(SS, LOW); // SS is pin 10

  String message = "";

  imu::Quaternion quat = bno.getQuat();
  message += String(quat.x(), 4);
  message += String(",");
  message += String(quat.y(), 4);
  message += String(",");
  message += String(quat.z(), 4);
  message += String(",");
  message += String(quat.w(), 4);
  message += "\n";

  // send our sensor data
  SPI.transfer(message.c_str(), message.length() + 1);

  // disable Slave Select
  digitalWrite(SS, HIGH);

  //Serial.println(message);

  delay (60);
}
