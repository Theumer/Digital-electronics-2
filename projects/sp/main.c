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
 *
 * 
 */ 

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>                         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "timer.h"
#include "uart.h"
#include "twi.h"
#include "nokia5110.h"
#include "gpio.h"

// DEFINE ------------------------------------------------------------
#define UART_BAUD_RATE 9600

// INIT OF GLOBAL VARIABLES
volatile uint8_t    data[400] = {0};
volatile uint8_t    identifier[6] = {0};
volatile uint8_t    i = 0;
uint16_t            baudrate = 9600;

volatile uint8_t    result;
volatile uint8_t    j = 0;
volatile uint8_t    state = 4;

char                receive;
char                NMEA[5][100];                      //pole k ukládání dat z GPS
char                indent[7];
char                latitude[12];
char                longitude[13];
char                time[9];
char                speed[6];

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program.
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

    // global eramenable of interrupt
    sei();

    //char uart_string[4];

    // LOOP FOR CHECKING SET OF RXC1 BIT
    while(1){
        // doing, while UDR1 receive reg is full (set RXC1 flag)
        while (!(UCSR1A & (1<<RXC1)))
        {   

            receive = uart1_getc();
            uart_putc(receive);

            if(receive == '$')
            {
                i = 0;
                state = 0;
            }

            if(i == 6)
            {
                state = 1;
                indent[i] = '\0';
                result = strcmp(indent, "$GPGGA");
                if(result == 0){
                    j = 0;
                }

                result = strcmp(indent, "$GPGSA");
		        if(result == 0) j = 1;

                result = strcmp(indent, "$GPGSV");
		        if(result == 0) j = 2;

                result = strcmp(indent, "$GPRMC");
		        if(result == 0) j = 3;

                result = strcmp(indent, "$GPVTG");
		        if(result == 0) j = 4;
            }

            switch(state)
            {
                case 0: indent[i] = receive; i++; break;
                case 1: NMEA[j][i-6] = receive; i++; break;
                default: break;
            }
        }
    }
    // infinite loop
   /* for (;;) {
    }
    return (0);*/
}

/* INTERRUPT VECTOR FOR REFRESH OF DISPLAY */


ISR(TIMER1_OVF_vect){
    memcpy(time, &NMEA[0][1], 6);
    time[8] = '\0';
    time[7] = time[5];
    time[6] = time[4];
    time[5] = ':';
    time[4] = time[3];
    time[3] = time[2];
    time[2] = ':';

    memcpy(latitude, &NMEA[0][12], 12);
    latitude[11] = '\0';

    memcpy(longitude, &NMEA[0][24], 13);
    longitude[12] = '\0';

    memcpy(speed, &NMEA[3][39], 4);
    speed[5] = '\0';

    nokia_lcd_clear();
    nokia_lcd_write_string(speed, 2);

    nokia_lcd_set_cursor(0, 20);
    nokia_lcd_write_string(latitude, 1);

    nokia_lcd_set_cursor(0, 30);
    nokia_lcd_write_string(longitude, 1);

    nokia_lcd_set_cursor(0, 40);
    nokia_lcd_write_string(time, 1);
    nokia_lcd_render(); 
}
