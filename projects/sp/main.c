/*
 * ---------------------------------------------------------------------
 * SEMESTRAL PROJECTS: BPC-DE2
 * Communication with GPS module
 *
 * Created:     2017-10-27
 * Last update: 2019-11-08
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "uart.h"
#include "twi.h"
#include "nokia5110.h"
#include "gpio.h"

/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------
state_t current_state = IDLE_STATE;
uint8_t temp;
uint8_t time_sec;
uint8_t time_min;*/
uint8_t data[65] = {0};
uint8_t receive;
uint8_t rec_flag;

/* Function prototypes -----------------------------------------------*/
/* void fsm_twi_scanner(void);*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Test all TWI slave addresses and scan 
 *          connected devices.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // INIT LCD NOKIA5110
    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_write_string("Init OK", 2);
    nokia_lcd_render();

    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    TIM_config_prescaler(TIM1, TIM_PRESC_256);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // INIT OF UART COMMUNICATION
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    uart1_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    uart_puts("\r\n---TEST UART---");

    sei();

    uint8_t i = 0;
    char uart_string[8];

    if((0x0048 == 1)&(i<65))
    {
        rec_flag = 0;
        data[i] = receive;
        itoa(data[i], uart_string, 10);
        uart_puts(uart_string);
        i++;

        /*for(i=0; i<65; i++)
        {
            data[i] = uart1_getc();
            itoa(data[i], uart_string, 10);
            uart_puts(uart_string);
        }*/
    }


    // infinite loop
    for (;;) {
    }
    
    return (0);
}

ISR(USART_RXC_vect)
{
    receive = uart1_getc();
    rec_flag = 1;
}