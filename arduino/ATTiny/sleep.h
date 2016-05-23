int wakeupCount = 0;

ISR(WDT_vect) {
  wakeupCount++;
}

void sleep_init() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //Power down everything, wake up from WDT
  sleep_enable();
}

void disable_adc() {
  ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA
}

void enable_adc() {
  ADCSRA |= (1<<ADEN); //Enable ADC
}

void setup_watchdog(int timerPrescaler) {
  if (timerPrescaler > 9) timerPrescaler = 9; //Limit incoming amount to legal settings

  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); //Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each interrupt
}

void sleep(int seconds) {
  int sleepCycles;
  int timerScaler = 9; // 8 seconds
  if (seconds < 8) {
    timerScaler = 6; // 1 seconds
    sleepCycles = seconds;
  } else {
    sleepCycles = seconds / 8;
  }

  disable_adc();

  wakeupCount = 0;
  while (wakeupCount < sleepCycles) {
   setup_watchdog(timerScaler); //Setup watchdog to go off after a hwile
   sleep_mode(); //Go to sleep! Wake up a little later
  }

  enable_adc();
}
