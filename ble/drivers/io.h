#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

// IO address = 010 0001 R/!W
// Write=0 Read=1
#define IO_SUBORDINATE_ADDR_W   0x21
#define IO_SUBORDINATE_ADDR_R   0x21
#define IO_CONFIG_REG           0x03
#define IO_OUTPUT_REG           0x01
#define IO_INPUT_REG            0x00

/**
 * Set all pins as output pins
 * 
 * @param val The value to be written
 * 
*/
void io_expander_set_configuration(uint8_t val);

/**
 * Set the the LEDs to be enabled
 * 
 * @param val
 * 
*/
void io_expander_set_output_port(uint8_t val);

/**
 * Get current status of output register
 * 
 * @param
 * 
*/
uint8_t io_expander_get_curr_output(void);

/**
 * Get current status of input register
 * 
 * @param
 * 
*/
uint8_t io_expander_get_curr_input(void);

/**
 * Turn decimal into bit mask for the LED
 * 
 * @param val
 * 
*/
uint8_t get_bit_mask(int val);

#endif
