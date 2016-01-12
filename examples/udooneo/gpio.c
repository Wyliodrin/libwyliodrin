/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: December 2016
 *************************************************************************************************/

#include <stdio.h>

#include <Wyliodrin.h>

#define PIN_OUT 25
#define PIN_IN  149

#define DELAY    500
#define NUM_ITER  10

int main() {
  pinMode(PIN_OUT, OUTPUT);
  pinMode(PIN_IN,  INPUT);

  int i;
  int value;
  for (i = 0; i < NUM_ITER; i++) {
    digitalWrite(PIN_OUT, HIGH);
    value = digitalRead(PIN_IN);
    printf("value = %d, expected 1", value);
    delay(DELAY);
    digitalWrite(PIN_OUT, LOW);
    value = digitalRead(PIN_IN);
    printf("value = %d, expected 0", value);
    delay(DELAY);
  }
}
