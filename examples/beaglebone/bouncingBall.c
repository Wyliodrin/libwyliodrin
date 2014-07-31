/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 *
 * Simulates a bouncing ball on the 4 on-board leds
 *************************************************************************************************/

#include <Wyliodrin/wiring.h>
#include <Wyliodrin/beagleboneConfig.h>

#define INITIAL_MILLIS 500
#define STEP           50

int main() {
  int millis;
  int USR0, USR1, USR2, USR3;

  USR0 = getPinByKey("USR0");
  USR1 = getPinByKey("USR1");
  USR2 = getPinByKey("USR2");
  USR3 = getPinByKey("USR3");

  pinMode(USR0, OUTPUT);
  pinMode(USR1, OUTPUT);
  pinMode(USR2, OUTPUT);
  pinMode(USR3, OUTPUT);

  for(millis = INITIAL_MILLIS; millis > 0; millis -= STEP) {
    digitalWrite(USR0, HIGH);
    delay(millis);
    digitalWrite(USR0, LOW);
    digitalWrite(USR1, HIGH);
    delay(millis);
    digitalWrite(USR1, LOW);
    digitalWrite(USR2, HIGH);
    delay(millis);
    digitalWrite(USR2, LOW);
    digitalWrite(USR3, HIGH);
    delay(millis);
    digitalWrite(USR3, LOW);
    digitalWrite(USR2, HIGH);
    delay(millis);
    digitalWrite(USR2, LOW);
    digitalWrite(USR1, HIGH);
    delay(millis);
    digitalWrite(USR1, LOW);
  }

  pinReset(USR0);
  pinReset(USR1);
  pinReset(USR2);
  pinReset(USR3);
  return 0;
}