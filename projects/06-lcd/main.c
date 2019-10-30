/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-16
 * Last update: 2019-10-25
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Decimal counter with data output on LCD display.
 * 
 * Note:
 *    Modified version of Peter Fleury's LCD library with R/W pin 
 *    connected to GND. Newline symbol "\n" is not implemented, use
 *    lcd_gotoxy() function instead.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>             // itoa() function
#include "timer.h"
#include "lcd.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/



/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on LCD display.
 *  Input:  None
 *  Return: None
 */
uint8_t value = 0;
uint8_t i;
char lcd_string[9];

int main(void)
{
    /* LCD display
     * TODO: See Peter Fleury's online manual for LCD library 
     * http://homepage.hispeed.ch/peterfleury/avr-software.html
     * Initialize display and test different types of cursor */
    lcd_init(LCD_DISP_ON);

    // Display string without auto linefeed
    //lcd_puts("LCD testing");

    // TODO: Display variable value in decimal, binary, and hexadecimal
    itoa(value, lcd_string, 10);
    lcd_gotoxy(0,0);
    lcd_puts("Counter:");
    lcd_gotoxy(9,0);
    lcd_puts(lcd_string);

    lcd_gotoxy(0,1);
    itoa(value, lcd_string, 16);
    lcd_putc('$');
    lcd_gotoxy(1,1);
    lcd_puts(lcd_string);

    lcd_gotoxy(6,1);
    itoa(value, lcd_string, 2);
    lcd_puts("0b");
    lcd_gotoxy(8,1);
    lcd_puts(lcd_string);

    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_256);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    /* TODO: Design at least two user characters and store them in 
     *       the display memory */
     uint8_t lcd_user_symbols[8*2] = {0b00010,
	                                    0b00110,
	                                    0b01110,
	                                    0b11110,
	                                    0b11110,
	                                    0b01110,
	                                    0b00110,
	                                    0b00010,
	                                    0b00010,
	                                    0b01001,
	                                    0b00101,
	                                    0b10101,
	                                    0b00101,
	                                    0b01001,
	                                    0b00010,
	                                    0b00000};
    
    // Set  pointer  to  beginning  of CG RAM  memory
    lcd_command(1<<LCD_CGRAM);
    
    // Store  new  characters  line by line
    for(i=0; i<16; i++) {
        lcd_data(lcd_user_symbols[i]);
    }
    
    // Clear  display  and  set  cursor  to home  position
    //lcd_clrscr ();
    //...

    //  Display  first  user -defined  character
    lcd_gotoxy(14,0);
    lcd_putc(0x00);
    lcd_gotoxy(15,0);
    lcd_putc(0x01);

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Increment counter value.
 */
ISR(TIMER1_OVF_vect)
{
    itoa(value, lcd_string, 10);
    lcd_gotoxy(9,0);
    lcd_puts("   ");
    lcd_gotoxy(9,0);
    lcd_puts(lcd_string);

    itoa(value, lcd_string, 16);
    lcd_gotoxy(1,1);
    lcd_puts("  ");
    lcd_gotoxy(1,1);
    lcd_puts(lcd_string);

    itoa(value, lcd_string, 2);
    lcd_gotoxy(8,1);
    lcd_puts("        ");
    lcd_gotoxy(8,1);
    lcd_puts(lcd_string);

    value++;
}
