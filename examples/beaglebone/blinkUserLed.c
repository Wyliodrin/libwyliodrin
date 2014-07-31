/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 *
 * Turns ON and OFF on-board led USR0 for 10 times at 0.5 secs interval
 *************************************************************************************************/

#include <Wyliodrin/wiring.h>
#include <Wyliodrin/beagleboneConfig.h>

#define NO_BLINKS 10

int main() {
  int i;
  int USR0;

  USR0 = getPinByKey("USR0");

  pinMode(USR0, OUTPUT);

  for(i = 0; i < NO_BLINKS; i++) {
    digitalWrite(USR0, HIGH);
    delay(500);
    digitalWrite(USR0, LOW);
    delay(500);
  }

  pinReset(USR0);
  return 0;
}