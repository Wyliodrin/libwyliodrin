/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 *
 * Simulates a police siren with a fading led.
 *
 * CIRCUIT: GND connected to one end of 10 kohm resistance
 *          the other end of resistance connected to led's cathode (-) (shorted leg)
 *          the led's anode (+) (the longer leg) connected to P9_14
 *************************************************************************************************/

#include <Wyliodrin/wiring.h>
#include <Wyliodrin/beagleboneConfig.h>

#define NO_BLINKS 10;

int main() {
  int i;
  int value;
  int P9_14;

  P9_14 = getPinByKey("P9_14");

  pinMode(P9_14, OUTPUT);

  for(i = 0; i < NO_BLINKS; i++) {
    for(value = 0; value <= 255; value += 10) {
      analogWrite(P9_14, value);
      delay(100);
    }
    for(; value >= 0; value -= 10) {
      analogWrite(P9_14, value);
      delay(100);
    }
  }

  pinReset(P9_14);
  return 0;
}