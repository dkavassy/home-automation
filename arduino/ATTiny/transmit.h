#include <VirtualWire.h>

//const int led_pin = 1;
const int transmit_pin = 0;
const int transmit_en_pin = 1;

void transmitter_init()
{
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_setup(2000);   // Bits per sec
}

void transmit(char* msg, unsigned int len) {
  //digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, len);
  vw_wait_tx(); // Wait until the whole message is gone
  //digitalWrite(led_pin, LOW);
}
