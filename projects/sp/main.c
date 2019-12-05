/*
 * ---------------------------------------------------------------------
 * SEMESTRAL PROJECTS: BPC-DE2
 * Communication with GPS module
 *
 *      Radek Theumer
 *      Jindrich Zabka
 *      Jonas Hubacek
 *
 * Created:     2017-10-27
 * Last update: 2019-12-05
 * Platform:    ATmega2560, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */

 /* EXAMPLES OF 5 NMEA SENTENCES:
 * $GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F
 * 
 */ 

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>                         // itoa() function
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

// INIT OF ARRAYS FOR 5 DATA PACKET FROM GPS
volatile uint8_t data[400] = {0};
volatile uint8_t GGA[100] = {0};
volatile uint8_t GSA[100] = {0};
volatile uint8_t GSV[100] = {0};
volatile uint8_t RMC[100] = {0};
volatile uint8_t VTG[100] = {0};
volatile uint8_t identifier[6] = {0};

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
    /* INIT OF LCD NOKIA5110 
    *       -init of SPI communication
    *       -send an introduce strings */
    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_write_string("Init OK", 2);

    nokia_lcd_set_cursor(0, 20);
    nokia_lcd_write_string("BPC-DE2", 1);

    nokia_lcd_set_cursor(0, 30);
    nokia_lcd_write_string("GPS module", 1);

    nokia_lcd_set_cursor(0, 40);
    nokia_lcd_write_string("VUT FEKT", 1);
    nokia_lcd_render();

    /* INIT OF UART COMMUNICATION
    *       USART0: communication AVR-PC via USB (port ttyACM0)
    *       USART1: communication with GPS module
    *       setting:    bit U2X = 0
    *                   Baud rate = 9600 (recommandation for GPS) */ 
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    uart1_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    /* INIT OF TIMER1
    *       -prescaler: 8
    *       -enable interrupt from overflow */
    TIM_config_prescaler(TIM1, TIM_PRESC_256);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // global enable of interrupt
    sei();


    uint8_t i = 0;
    //char uart_string[4];

    // LOOP FOR CHECKING SET OF RXC1 BIT
    while(1){
        // doing, while UDR1 receive reg is full (set RXC1 flag)
        while (!(UCSR1A & (1<<RXC1)))
        {
            data[i] = uart1_getc();
            if(data[i] != 36)               // if char is not '$' - find a start of packet
            {
                i = 0;
                break;
            }
            identifier[i] = data[i];
            uart_putc(data[i]);
            i++;
        }
    }

    // infinite loop
    for (;;) {
    }
    
    return (0);
}

/* INTERRUPT VECTOR FOR REFRESH OF DISPLAY */
ISR(TIMER1_OVF_vect){
    nokia_lcd_clear();
    nokia_lcd_write_string("SPEED", 2);

    nokia_lcd_set_cursor(0, 20);
    nokia_lcd_write_string("latitude", 1);

    nokia_lcd_set_cursor(0, 30);
    nokia_lcd_write_string("longitude", 1);

    nokia_lcd_set_cursor(0, 40);
    nokia_lcd_write_string("hh:mm:ss", 1);
    nokia_lcd_render();
}
