
# SEMESTRAL PROJECT

#### Table of Contents

1. [Introduction](#introduction)
2. [Hardware](#hardware)
3. [Software](#software)
4. [Commits](#commits)
5. [References](#references)


## Introduction

***Semestral project in BPC-DE2: Communication with GPS module***  
  
Authors:  
 - Radek Theumer  
 - Jindrich Zabka  
 - Jonas Hubacek


## Hardware

- AVR ATmega2560, 8-bit controller
- PA6H GPS module
- Nokia5110 LCD display

> We use ATmega2560 kit, because this controller provides four USARTs. To communication with PC we use USART0 (connect with USB port usbACK0) and to receive data from GPS we use USART1.


## Software development

Creating application:  
- [x] LCD Nokia5110 running (28th Nov)  
- [x] USAT0 and USART1 communication with GPS module and PC running (5th Dec)  
- [x] assignment receive data into arrays (11th Dec)
- [x] test of GPS outside (17th Dec)
  
We use interrupt routine from timer1 - overflow. Prescaler of 8bit timer1 is set to 256 (about every 1 second). In this interrupt routine we refresh LCD display. At first, we create string from concrete to concrete index of corresponding row (of 2D array) with name of NMEA sentence. We create it with help of function "memcpy", which is contained in library string.h.  
<img src="https://github.com/Theumer/Digital-electronics-2/tree/master/docs/nmea_sent.png">  

## Commits

[LCD]   ... programming LCD disp.  
[USART] ... programming USART communication  
[GPS]   ... reading GPS data from PA6H  
[TEST]  ... test application outside  
[.MD]   ... edit of .md file  
[NOTE]  ... edit of comments and notes


## References

1. Microchip, [AVR 8-bit Toolchain for Windows, Linux, Mac OS X](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers)  
2. Sergey Denisov, online, [C library for control UART, AVR](https://github.com/LittleBuster/avr-uart/tree/master/mega328)  
3. Datasheet PA6h GPS module, online, [available here](https://cdn-shop.adafruit.com/datasheets/GlobalTop-FGPMMOPA6H-Datasheet-V0A.pdf)
