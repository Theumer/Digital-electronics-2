
# SEMESTRAL PROJECT

#### Table of Contents

1. [Introduction](#introduction)
2. [Hardware](#hardware)
3. [Software](#software)
4. [Commits](#commits)
5. [References](#references)


## Introduction

***Semestral project in BPC-DE2: Communication with GPS module***  
  
Authors: Radek Theumer, Jindrich Zabka, Jonas Hubacek  
  
Department of radioelectronics  
BUT Brno, FEEC


## Hardware

- AVR ATmega2560, 8-bit controller
- PA6H GPS module
- Nokia5110 LCD display

> We use ATmega2560 kit, because this controller provides four USARTs. To communication with PC we use USART0 (connect with USB port usbACM0) and to receive data from GPS we use USART1.  
  
Schematic of embedded device:  
<img src="https://github.com/Theumer/Digital-electronics-2/blob/master/projects/sp/embedded_schema.png" width="500">  


## Software development

Creating application step-by-step:  
- [x] LCD Nokia5110 running (28th Nov)  
- [x] USAT0 and USART1 communication with GPS module and PC running (5th Dec)  
- [x] assignment receive data into arrays (11th Dec)
- [x] test of GPS outside (17th Dec)
  
  
#### About devices  
We use interrupt routine from timer1 - overflow. Prescaler of 8bit timer1 is set to 256 (about every 1 second). In this interrupt routine we refresh LCD display. At first, we create string from concrete to concrete index of corresponding row (of 2D array) with name of NMEA sentence. We create it with help of function "memcpy", which is contained in library string.h.  
<img src="https://github.com/Theumer/Digital-electronics-2/blob/master/projects/sp/nmea_sent.png">  
  
GPS module PA6H send data continuously. After connect to supply, we must wait about 35 seconds to finish internal init of GPS module. [3] Every 1 second module send four NMEA sentences: GPGGA, GPGSA, GPGSV and GPRMC. Every 5 second we receive one more sentence, which call GPVTG. We using data only from GPGGA and GPRMC sentences.  
  
ATmega2560 has 4 UARTs. We need 2 UARTs chanel to communication with PA6H and with computer at the same time. Set Baud rate at both UARTs is 9600 Bd. In PC we display received NMEA sentences in PuTTy.
  
  
#### Flowchart   
Our algorithm work as state automat. First state "0" indetifies received sentence. Succesfully identification leds to state "1". There is assigned data to corresponding row of 2D array. Displaying of data (take from/to concrete index of 2D array) is controlling by TIMER1 overflow interrupt routine.
  
<img src="https://github.com/Theumer/Digital-electronics-2/blob/master/projects/sp/flowchart.jpg" width="500">  

  
## Commits

[LCD]   ... programming LCD disp.  
[USART] ... programming USART communication  
[GPS]   ... reading GPS data from PA6H  
[TEST]  ... test application outside  
[.MD]   ... edit of .md file  
[NOTE]  ... edit of comments and notes


## References and links

1. Microchip, [AVR 8-bit Toolchain for Windows, Linux, Mac OS X](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers)  
2. Sergey Denisov, online, [C library for control UART, AVR](https://github.com/LittleBuster/avr-uart/tree/master/mega328)  
3. Datasheet PA6h GPS module, online, [available here](https://cdn-shop.adafruit.com/datasheets/GlobalTop-FGPMMOPA6H-Datasheet-V0A.pdf)
4. Datasheet Nokia5110 LCD display, online, [available here](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)  
  
PDF [presentation](https://github.com/Theumer/Digital-electronics-2/blob/master/projects/sp/GPS_Semestral_project.pdf)  
YouTube [video](https://www.youtube.com/watch?v=nIREcNWi07I)  
Flowchart in [vector](https://github.com/Theumer/Digital-electronics-2/blob/master/projects/sp/vyvojovy_diagram_de2.pdf)
