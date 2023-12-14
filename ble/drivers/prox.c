/*
 * prox.c
 *
 *  Created on: Nov 13, 2023
 *      Author: Cade Kenyon, and Kayla Thrane 
 */

#include "prox.h"
#include "io.h"
#include "drive.h"

cy_rslt_t prox_init(void)
{
    cy_rslt_t rslt;
    io_expander_set_configuration(0xF0);
    return rslt;
}

uint8_t prox_read0(void)
{
    uint8_t in = io_expander_get_curr_input();
    uint8_t mask = 0xF0;
    return in & mask;
} 

void prox_shutdown(void) {
    uint8_t prox_reading = prox_read0();
    uint8_t masked_warning;

    if (bogo_dir == 'w') {
        masked_warning = prox_reading & 0x10;
        if (masked_warning == 0x00) {
            drive_dir(0x34);
        }

    } else if (bogo_dir == 's') {
        masked_warning = prox_reading & 0x40;
        if (masked_warning == 0x00) {
            drive_dir(0x34);
        }

    } else if (bogo_dir == 'a') {
        masked_warning = prox_reading & 0x20;
        if (masked_warning == 0x00) {
            drive_dir(0x34);
        }

    } else if (bogo_dir == 'd') {
        masked_warning = prox_reading & 0x80;
        if (masked_warning == 0x00) {
            drive_dir(0x34);
        }
    }

}