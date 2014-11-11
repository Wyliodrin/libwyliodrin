/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 *
 * Turns ON and OFF on-board led USR0 for NO_BLINKS times at DELAY millis interval
 *************************************************************************************************/

#include <Wyliodrin/wiring.h>
#include <Wyliodrin/beagleboneConfig.h>

#define NO_BLINKS 10
#define DELAY     500

int main() {
  int i;

  pinMode(0, OUTPUT);

  for(i = 0; i < NO_BLINKS; i++) {
    digitalWrite(USR0, HIGH);
    delay(DELAY);
    digitalWrite(USR0, LOW);
    delay(DELAY);
  }

  pinReset(USR0);
  return 0;
}