/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: December 2016
 *************************************************************************************************/

#include <Wyliodrin.h>

#define PIN_OUT 25
#define PIN_IN  149

#define DELAY    500
#define NUM_ITER  10

int main() {
  pinMode(PIN_OUT, OUTPUT);

  int i;
  for (i = 0; i < NUM_ITER; i++) {
    digitalWrite(PIN_OUT, HIGH);
    delay(DELAY);
    digitalWrite(PIN_OUT, LOW);
    delay(DELAY);
  }
}
