/*
 * -----------------------------------------------------------------------------------------
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
 *
 * BUT Brno, FEEC
 * Department of radioelectronics
 * -----------------------------------------------------------------------------------------
 */

 /* EXAMPLES OF 5 NMEA SENTENCES THAT PA6H SENDS
 *$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47	
 *$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39
 *$GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75
 *$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
 *$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
*/ 

/* Includes ----------------------------------------------------------------------------*/
#include <stdlib.h>                         
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "timer.h"
#include "uart.h"
#include "twi.h"
#include "nokia5110.h"
#include "gpio.h"

// DEFINE ------------------------------------------------------------------------------
#define UART_BAUD_RATE 9600

// INIT OF GLOBAL VARIABLES
volatile uint8_t    i = 0;
volatile uint8_t    result;
volatile uint8_t    j = 0;
volatile uint8_t    state = 4;

char                receive;
char                NMEA[5][100];       // 2D array for data 
char                indent[7];		// array used for picking NMEA sentence					
char                latitude[12];			
char                longitude[13];
char                time[9];
char                speed[6];


int main(void)
{
    /* INIT OF LCD NOKIA5110 
    *       initialization of SPI communication
    *       send some introducing strings 
    */
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
    *                   Baud rate = 9600 (recommandation for GPS) 
    */  
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    uart1_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    /* INIT OF TIMER1
    *       -prescaler: 256
    *       -enable interrupt from overflow 
    */
    TIM_config_prescaler(TIM1, TIM_PRESC_256);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);	

    // global enable of interrupt
    sei();

    // LOOP FOR CHECKING SET OF RXC1 BIT
    while(1){
        // doing, while UDR1 receive reg is full (set RXC1 flag)
        while (!(UCSR1A & (1<<RXC1)))
        {   
	    // copy received byte and send to UART0 (to PC)
            receive = uart1_getc();
            uart_putc(receive);

	    // checking char '$': begin new data frame
            if(receive == '$')
            {
                i = 0;
                state = 0;
            }

	    // step up state automat (data into 2D array)
            if(i == 6)
            {
                state = 1;
		// creating a string adding NULL to last position
                indent[i] = '\0';

		// compare with original name, if result == 0, it is match
                result = strcmp(indent, "$GPGGA");
                	if(result == 0) j = 0;

		// doing for all 5 data frames
                result = strcmp(indent, "$GPGSA");
		        if(result == 0) j = 1;

                result = strcmp(indent, "$GPGSV");
		        if(result == 0) j = 2;

                result = strcmp(indent, "$GPRMC");
		        if(result == 0) j = 3;

                result = strcmp(indent, "$GPVTG");
		        if(result == 0) j = 4;
            }

	    // state automat: 	0 = identification sentence
	    //			1 = assignment data into corresponding row
            switch(state)
            {
                case 0: indent[i] = receive; i++; break;
                case 1: NMEA[j][i-6] = receive; i++; break;
                default: break;
            }
        }
    }
}


/* INTERRUPT VECTOR FOR REFRESH OF DISPLAY (1x per sec.) */

ISR(TIMER1_OVF_vect){
    // time seperating to xx:xx:xx
    memcpy(time, &NMEA[0][1], 6);
    time[8] = '\0';
    time[7] = time[5];
    time[6] = time[4];
    time[5] = ':';
    time[4] = time[3];
    time[3] = time[2];
    time[2] = ':';

    // CREATE STRINGs FROM FRAGMENTS OF 2D ARRAY
    // value for latitude
    memcpy(latitude, &NMEA[0][12], 12);
    latitude[11] = '\0';

    // value for longitude
    memcpy(longitude, &NMEA[0][24], 13);
    longitude[12] = '\0';

    // value for speed
    memcpy(speed, &NMEA[3][39], 4);
    speed[5] = '\0';

    // DISPLAYING DATA - REFRESH LCD
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
