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
/*
#define pin_CE      PD3
#define pin_DC      PB2
#define pin_RST     PD4

#define SPIE        7       // SPIE bit: interrupt enable
#define SPE         6       // SPI communication enable
#define MSTR        4       // set ATmega328P as master
#define SPR0        0       // set frequency
#define MOSI        PB3     // data input (Master output-Slave input)
#define SCK         PB5 */

/* Variables ---------------------------------------------------------
state_t current_state = IDLE_STATE;
uint8_t temp;
uint8_t time_sec;
uint8_t time_min;*/

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
    nokia_lcd_write_string("Horse", 2);
    nokia_lcd_render();

    // Infinite loop
    for (;;) {
    }
    
    // Will never reach this
    return (0);
}