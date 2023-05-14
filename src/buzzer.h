// #include <avr/io.h>
// #include <stdint.h>
#include <util/delay.h>
// #include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "conf.h"
#ifndef	BUZZER_PIN
#define BUZZER_PIN PB0
#endif	/* !BUZZER_PIN */

void tone(uint8_t delay_ms, uint8_t delay_us);
#ifdef ATTINY85
  void start_tone(uint8_t octave, uint8_t note);
  void stop_tone();
#endif