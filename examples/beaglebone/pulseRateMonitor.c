/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 *
 * Checks your pulse rate.
 *
 * CIRCUIT: connect one end of a father-father connector to P9_40 (AIN1)
 *          place the other end on a visible artery from your wrist
 *************************************************************************************************/

#include <time.h>
#include <stdio.h>
#include <Wyliodrin/wiring.h>
#include <Wyliodrin/beagleboneConfig.h>

#define SECONDS    60
#define LOW_LIMIT  10
#define HIGH_LIMIT 90

int main() {
  clock_t tStart;
  bool isUp;
  int value;
  int AIN1;
  int cnt;

  printf("*** Pulse Rate Monitor ***\n");

  AIN1 = getPinByName("AIN1");
  pinMode(AIN1, INPUT);

  cnt = 0;
  isUp = false;
  tStart = clock();
  while(100 * (clock() - tStart) / CLOCKS_PER_SEC < SECONDS) {
    value = analogRead(AIN1);
    if(!isUp && value > HIGH_LIMIT) {
      printf("TIC\n");
      isUp = true;
      cnt++;
    } else if(isUp && value < LOW_LIMIT) {
      printf("TOC\n");
      isUp = false;
      cnt++;
    }
    delay(100);
  }

  printf("Pulse rate: %d beats per minute\n", cnt);

  pinReset(AIN1);
  return 0;
}