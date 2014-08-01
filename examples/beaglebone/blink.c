/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 *
 * Turns ON and OFF an external led for NO_BLINKS times at DELAY millis interval
 *
 * CIRCUIT: GND connected to one end of 10 kohm resistance
 *          the other end of resistance connected to led's cathode (-) (shorted leg)
 *          the led's anode (+) (the longer leg) connected to P8_3
 *************************************************************************************************/

#include <Wyliodrin/wiring.h>
#include <Wyliodrin/beagleboneConfig.h>

#define NO_BLINKS 10
#define DELAY     500

int main() {
  int i;
  int pin;

  pin = getPinByKey("P8_3");

  pinMode(pin, OUTPUT);

  for(i = 0; i < NO_BLINKS; i++) {
    digitalWrite(pin, HIGH);
    delay(DELAY);
    digitalWrite(pin, LOW);
    delay(DELAY);
  }

  pinReset(pin);
  return 0;
}