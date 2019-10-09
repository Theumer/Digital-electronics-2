#include "gpio.h"
void  GPIO_config_output(volatile  uint8_t *reg_name , uint8_t  pin_num)
{
    *reg_name  |= _BV(pin_num);
}

void  GPIO_toggle(volatile  uint8_t *reg_name , uint8_t  pin_num)
{
    *reg_name ^= ~_BV(pin_num);
}

void  GPIO_write(volatile  uint8_t *reg_name , uint8_t  pin_num, uint8_t pin_val)
{
    if(pin_val == 0)
        *reg_name &= ~_BV(pin_num);
    else
        *reg_name |= _BV(pin_num);
}

void GPIO_input_pullup(volatile  uint8_t *reg_name , uint8_t  pin_num)
{
    DDR(*reg_name) &= ~_BV(pin_num);
    *reg_name |= _BV(pin_num);
}