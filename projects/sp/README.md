
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


## Software

TBD


## Commits

[LCD]   ... programming LCD disp.  
[USART] ... programming USART communication  
[GPS]   ... reading GPS data from PA6H  
[TEST]  ... test application outside  
[.MD]   ... edit of .md file  


## References

1. Microchip, [AVR 8-bit Toolchain for Windows, Linux, Mac OS X](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers)
