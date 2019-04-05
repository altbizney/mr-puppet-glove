#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

char buf [500];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (250000);
  while (!Serial);

  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // grab byte from SPI Data Register

  // add to buffer if room
  if (pos < (sizeof (buf) - 1))
    buf [pos++] = c;

  // example: newline means time to process buffer
  if (c == '\n')
    process_it = true;

}

void loop (void)
{
  if (process_it)
  {
    buf [pos] = 0;
    Serial.println (buf);
    pos = 0;
    process_it = false;
  }
}
