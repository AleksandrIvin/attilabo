/**
 * IR Remote for ATtiny85
 */

#include <stdint.h>
#include <avr/io.h>

#ifndef _IR_H_
#define _IR_H_

#include "conf.h"
// IR pin Attiny85 = PB2 (7 pin)
// //#define IR_IN_PIN PB2
// 9.6Mhz Attiny13 9.600.000 / 38222 / 2 = 125  (I don't know why 122)
// #define	IR_OCR0A		(122)  // worked
// 8Mhz  Attiny85 8.000.000 / 38222 / 2 = 104
#define IR_OCR0A (104)

#define LOW (0)
#define HIGH (1)

#define IR_SUCCESS (0)
#define IR_ERROR (1)

#define IR_EVENT_IDLE (0)
#define IR_EVENT_INIT (1)
#define IR_EVENT_FINI (2)
#define IR_EVENT_PROC (3)

#define IR_PROTO_EVENT_INIT (0)
#define IR_PROTO_EVENT_DATA (1)
#define IR_PROTO_EVENT_FINI (2)
#define IR_PROTO_EVENT_HOOK (3)

// depends of the CPU overload
#define HALF_COUNTER_RANGE (15)
/* a 4.5ms space for regular transmition of NEC Code; counter => 0.0045/(1.0/38222.0) * 2 = 344 (+/- 15) */
#define IR_NEC_CODE_REG_TRANSM_4_5_MS (345)
// #define IR_NEC_CODE_REG_TRANSM_4_5_MS (290) // was worked
#define IR_NEC_CODE_REG_TRANSM_4_5_MS_L (IR_NEC_CODE_REG_TRANSM_4_5_MS - HALF_COUNTER_RANGE)
#define IR_NEC_CODE_REG_TRANSM_4_5_MS_H (IR_NEC_CODE_REG_TRANSM_4_5_MS + HALF_COUNTER_RANGE)

/* a 2.25ms space for NEC Code repeat; counter => 0.00225/(1.0/38222.0) * 2 = 172 (+/- 15) */
#define IR_NEC_CODE_REG_TRANSM_2_25_MS (172)
// #define IR_NEC_CODE_REG_TRANSM_2_25_MS (135) // was worked

/* a 9ms leading pulse burst, NEC Infrared Transmission Protocol detected,
        counter = 0.009/(1.0/38222.) * 2 = 343.998 * 2 = 686 (+/- 30) */
#define IR_NEC_CODE_PULSE_BURST_9_0_MS (686)
// #define IR_NEC_CODE_PULSE_BURST_9_0_MS (575) // was worked

#define IR_EVENT_IDLE_TIMEOUT (7400)
#define IR_COUNTER_LIMIT (10000)
// #define IR_COUNTER_LIMIT (8333) // was worked

void IR_init(void);
int8_t IR_read(uint8_t *address, uint8_t *command);

#endif /* !_IR_H_ */
