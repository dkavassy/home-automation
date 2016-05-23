#include <avr/sleep.h>
#include "sleep.h"
#include "transmit.h"

const char identifier = 0x01;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(0, OUTPUT);
  sleep_init();
  transmitter_init();
}

// the loop function runs over and over again forever
void loop() {
  char msg[] = { identifier };
  transmit(msg, 1);
  sleep(32);
}

