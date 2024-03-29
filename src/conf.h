#ifndef _CONF_H_
#define _CONF_H_

    #ifdef ATTINY85
        // Attiny85
        #ifndef F_CPU
            #define F_CPU 8000000UL
        #endif

        #define LED_PIN PB1
        #define IR_IN_PIN PB2
        #define UPTIME_TO_SLEEP ((int)(1.15 * 60 * 10))  // 10 min
    #endif

    #ifdef ATTINY13
        // Attiny13
        #ifndef F_CPU
            #define F_CPU 9600000UL
        #endif

        #define LED_PIN PB2
        #define IR_IN_PIN PB1
        #define UPTIME_TO_SLEEP ((int)(2 * 60 * 10))  // 10 min

    #endif

    #define BUZZER_PIN PB0
    #define MOTOR_F_PIN PB3
    #define MOTOR_B_PIN PB4
#endif    