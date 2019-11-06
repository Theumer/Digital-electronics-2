/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-23
 * Last update: 2019-11-01
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Analog-to-digital conversion with displaying result on LCD and 
 *    transmitting via UART.
 * 
 * Note:
 *    Peter Fleury's UART library.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>             // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600



/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Read ADC result and transmit via UART.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // LCD display
    lcd_init(LCD_DISP_ON);
     //lcd_puts("LCD testing");

    /* ADC
     * TODO: Configure ADC reference, clock source, enable ADC module, 
     *       and enable conversion complete interrupt */
    GPIO_write(&ADMUX, 6, 1); //ADMUX, bit 6 a 7, konfigurace referenčního napětí
    GPIO_write(&ADMUX, 7, 0);
    GPIO_write(&ADCSRA, 0, 1); //ADCSRA, bit 0,1,2; nastavení předděličky na nejvyšší hodnotu
    GPIO_write(&ADCSRA, 1, 1);
    GPIO_write(&ADCSRA, 2, 1);
    GPIO_write(&ADCSRA, 7, 1); //povolení ADC
    GPIO_write(&ADMUX, 0, 0); //nastavení kanálu ADC0
    GPIO_write(&ADMUX, 1, 0);
    GPIO_write(&ADMUX, 2, 0);
    GPIO_write(&ADMUX, 3, 0);
    GPIO_write(&ADCSRA, 3, 1); //nastavení complete interrupt ADC
    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);
    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put string to ringbuffer for transmitting via UART.
    uart_puts("UART testing\r\n");

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Start ADC conversion.
 */
ISR(TIMER1_OVF_vect)
{
    // TODO: Start ADC conversion
    GPIO_write(&ADCSRA, 6, 1);                                    
}

/**
 *  Brief: ADC complete interrupt routine. Update LCD and UART 
 *         transmiter.
 */
ISR(ADC_vect)
{
    uint16_t value = 0;
    char uart_string[4];

	// Read 10-bit ACD value
    value = ADC;
    
    // TODO: Update LCD and UART transmiter
    itoa(value, uart_string, 10);
    lcd_puts("        ");
    lcd_gotoxy(0,1);
    lcd_puts(uart_string);
    switch(value)
    {
        case 0:
            uart_puts("RIGHT\n\r");
            break;
        case 638:
            uart_puts("SELECT\n\r");
            break;
        case 409:
            uart_puts("LEFT\n\r");
            break;
        case 99:
            uart_puts("UP\n\r");
            break;
        case 256:
            uart_puts("DOWN\n\r");
            break;
    }
    //uart_puts(uart_string);
}
