/**
 * Copyright (c) 2022, Aleksandr Ivin
 * 
 * Thanks for Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ATtiny13 NEC proto analyzer, ATtiny13 tone generator
 * Example of monblocking IR signal reader (38kHz, TSOPxxx) and NEC protocol decoder. * 
 * https://blog.podkalicki.com/attiny13-tone-generator/
 * https://blog.podkalicki.com/attiny13-ir-receiver-nec-proto-analyzer/
 * https://blog.podkalicki.com/100-projects-on-attiny13/
 * 
 * Settings:
 *  FUSE_L=0x7A
 *  FUSE_H=0xFF
 *  F_CPU=8000000UL
 */

/*
Known Issuses:
1. start_tone doesn't work. it brokes IR timer. 
2. After IR timer inits INIT_IR, _delay() start working 2x slower

todo:
1. Is it possible to wakeup with IR ? I didn't have a success
2. Try to work at 1 Mhz
3. Find a more correctly way to switchoff. 

*/

//#define F_CPU 8000000UL // Attiny85

#include <util/delay.h>
#include "ir.h"
#include "buzzer.h"

// Infra red control button codes
#define IRC_BUTTON_1 0x45
#define IRC_BUTTON_2 0x46
#define IRC_BUTTON_3 0x47
#define IRC_BUTTON_4 0x44
#define IRC_BUTTON_5 0x40
#define IRC_BUTTON_6 0x43
#define IRC_BUTTON_7 0x07
#define IRC_BUTTON_8 0x15
#define IRC_BUTTON_9 0x09
#define IRC_BUTTON_STAR 0x16
#define IRC_BUTTON_0 0x19
#define IRC_BUTTON_HASH 0x0D
#define IRC_BUTTON_UP 0x18
#define IRC_BUTTON_LEFT 0x08
#define IRC_BUTTON_OK 0x1C
#define IRC_BUTTON_RIGHT 0x5A
#define IRC_BUTTON_DOWN 0x52

#define BUZZER_PIN PB0
#define LED_PIN PB1
#define MOTOR_F_PIN PB3
#define MOTOR_B_PIN PB4

#define MOTOR_STOP (0)
#define MOTOR_FORWARD (1)
#define MOTOR_BACK (2)

uint8_t motor_state = 0;
uint8_t setting_led = 1;
uint8_t setting_buzzer = 1;

void move(uint8_t direction)
{
    switch (direction)
    {
    case MOTOR_STOP:
        // STOP
        /* Stop motors, Leds Off */
        motor_state = 0;
        PORTB &= ~(_BV(BUZZER_PIN) | _BV(LED_PIN) | _BV(MOTOR_F_PIN) | _BV(MOTOR_B_PIN));
        break;
    case MOTOR_FORWARD:
        // Move Forward
        motor_state = 1;
        PORTB |= _BV(MOTOR_F_PIN);
        PORTB &= ~_BV(MOTOR_B_PIN);
        PORTB |= _BV(LED_PIN);
        break;
    case MOTOR_BACK:
        // Move Back
        motor_state = 2;
        PORTB |= _BV(MOTOR_B_PIN);
        PORTB &= ~_BV(MOTOR_F_PIN);
        PORTB &= ~_BV(LED_PIN);
        break;
    default:
        break;
    }
}

void flash_async()
{
    static uint16_t flash_cnt = 0;
    if (++flash_cnt == 24000)
    {
        if (setting_led)
            PORTB ^= _BV(LED_PIN); // toggle LED
        if (setting_buzzer)
            PORTB ^= _BV(BUZZER_PIN); // toggle BUZZER
        flash_cnt = 0;    
    }
}

void flash_led(uint8_t a)
{
    PORTB &= ~_BV(LED_PIN);
    for (uint8_t i = 0; i < a; i++)
    {
        PORTB |= _BV(LED_PIN);
        _delay_ms(100);
        PORTB &= ~_BV(LED_PIN);
        _delay_ms(100);
    }
}

void go_sleep()
{
    tone(200, 20);
    flash_led(5);
    // Turn off all the pins
    PORTB &= ~_BV(LED_PIN);
    PORTB &= ~_BV(MOTOR_F_PIN);
    PORTB &= ~_BV(MOTOR_B_PIN);

    MCUCR |= (1 << SM1); // enabling sleep mode and powerdown sleep mode
    MCUCR |= (1 << SE);  // Enabling sleep enable bit
    __asm__ __volatile__("sleep"
                         "\n\t" ::); // Sleep instruction to put controller to sleep
    // controller stops executing instruction after entering sleep mode
}

int main(void)
{
    uint8_t addr, cmd;
    volatile uint32_t cycle_tick_cnt = 0;
    volatile uint32_t uptime_s = 0;

    /* setup */
    DDRB |= _BV(BUZZER_PIN) | _BV(LED_PIN) | _BV(MOTOR_F_PIN) | _BV(MOTOR_B_PIN);
    PORTB &= ~_BV(LED_PIN);
    PORTB &= ~_BV(MOTOR_F_PIN);
    PORTB &= ~_BV(MOTOR_B_PIN);

    // tone(200, 10);  ///
    // PORTB ^= _BV(LED_PIN);
    // PORTB ^= _BV(MOTOR_B_PIN);
    // _delay_ms(5000);
    // PORTB ^= _BV(LED_PIN);
    // PORTB ^= _BV(MOTOR_B_PIN);
    // _delay_ms(2000);

    IR_init();
    // after init time is x2 slow
    // PORTB ^= _BV(LED_PIN);
    // PORTB ^= _BV(MOTOR_F_PIN);
    // _delay_ms(5000);
    // PORTB ^= _BV(LED_PIN);
    // PORTB ^= _BV(MOTOR_F_PIN);
    // _delay_ms(2000);
    // PORTB &= ~_BV(LED_PIN);
    // PORTB &= ~_BV(MOTOR_F_PIN);
    // PORTB &= ~_BV(MOTOR_B_PIN);
    tone(200, 10);
    flash_led(2);

    /* loop */
    while (1)
    {
        if (IR_read(&addr, &cmd) == IR_SUCCESS)
        {
            if (addr != 0x00)
            {
                continue;
            }

            switch (cmd)
            {
            case IRC_BUTTON_OK:
                move(MOTOR_STOP);
                break;
            case IRC_BUTTON_UP:
                move(MOTOR_FORWARD);
                break;
            case IRC_BUTTON_DOWN:
                move(MOTOR_BACK);
                break;
            case IRC_BUTTON_1:
                // Toggle Buzzer
                PORTB ^= _BV(BUZZER_PIN); // toggle LED1
                setting_buzzer = !setting_buzzer;
                break;
            case IRC_BUTTON_2:
                // Toggle LED
                PORTB ^= _BV(LED_PIN);
                setting_led = !setting_led;
                break;
            case IRC_BUTTON_STAR:
                // button *
                // Toggle LED
                go_sleep();
                break;
            default:
                // Unknown NEC code
                tone(200, 10);
                flash_led(3);
                // start_tone(1, 1);
                // _delay_ms(100);
                // stop_tone();
                break;
            };
            // reset uptime on last event
            uptime_s = 0;
        }

        if (motor_state == MOTOR_BACK)
        {
            flash_async();
        }

        if (cycle_tick_cnt++ > 100000) // 1 cycle is around 2s at 8Mhz
        {
            cycle_tick_cnt = 0;
            uptime_s++;
        }

        if (uptime_s > 300)  // ~600 seconds
        {
            go_sleep();
        }
    }
}
