#include <SPI.h>

void setup (void) {
  Serial.begin(115200);
  digitalWrite(SS, HIGH); // disable Slave Select
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop (void) {
  char c;
  digitalWrite(SS, LOW); // enable Slave Select
  // send test string
  for (const char * p = "464;-0.6227,-0.2369,0.7269,-0.1663,3,0,0,3;-0.6463,-0.2591,0.6964,-0.1741,3,0,0,2;-0.4869,-0.1858,0.8318,-0.1908,3,0,1,2\r"; c = *p; p++)
  {
    SPI.transfer(c);
    Serial.println(c);
  }
  digitalWrite(SS, HIGH); // disable Slave Select
  delay(60);
}
