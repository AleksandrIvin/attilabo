#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#ifndef	BUZZER_PIN
#define BUZZER_PIN PB0
#endif	/* !BUZZER_PIN */

void tone(int delay_ms, int delay_us);
void start_tone(int octave, int note);
void stop_tone();