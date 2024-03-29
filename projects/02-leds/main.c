/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-09-27
 * Last update: 2019-09-26
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Blink a LED with delay function.
 * TODO: Verify functionality of LED blinker.
 * NOTE: Macro _BV converts a bit number into a byte value (see AVR Libc
 * Reference Manual).
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include <gpio.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_GREEN   PB1
#define LED_RED     PB0
#define BLINK_DELAY 500
#define BUTTON      PD4

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
  * Brief:  Main program. Blink a LED with delay function.
  * Input:  None
  * Return: None
  */
int main(void)
{
    /* Set output pin */
    /*DDRB |= _BV(LED_GREEN);         /* DDRB = DDRB or (0010 0000) */
    GPIO_config_output(DDRB , LED_GREEN);
    /*DDRB |= _BV(LED_RED);*/
    GPIO_config_output(DDRB , LED_RED); 
    /*DDRD &= ~_BV(BUTTON), PORTD |= _BV(BUTTON);*/
    GPIO_input_pullup(PORTD, BUTTON);
    
    /* Turn LED off */
    /*PORTB &= ~_BV(LED_GREEN);       /* PORTB = PORTB and (0010 0000) */
    /*PORTB |= _BV(LED_RED); */
    GPIO_write(PORTB, LED_GREEN, 1);
    GPIO_write(PORTB, LED_RED, 0);

    /* Infinite loop */
    for (;;)
    {
      if(bit_is_clear(PIND, BUTTON))
      {
        /* Invert LED and delay */
        /*PORTB ^= _BV(LED_GREEN);    /* PORTB = PORTB xor (0010 0000) */
        /*PORTB ^= _BV(LED_RED);*/
        GPIO_toggle(PORTB, LED_GREEN);
        GPIO_toggle(PORTB, LED_RED);
        _delay_ms(BLINK_DELAY);     /* Wait for several milisecs */
      }
    }

    return (0);
}
