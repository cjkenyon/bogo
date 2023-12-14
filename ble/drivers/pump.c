/*
 * pump.c
 *
 *  Created on: Nov 13, 2023
 *      Author: Kayla Thrane 
 */

#include "pump.h"

#define SERVO_INC 10

static int servo_degree = 0;
/**
 * Turn the pump on and off
*/
void pump(int on_off) {
    uint8_t curr_output;
    uint8_t new_output;

    curr_output = io_expander_get_curr_output();
    
    if (on_off == 0x30) {
        // OFF -- 0x1111 1110
        new_output = curr_output & 0xFE;
        io_expander_set_output_port(new_output);

    }
    else if (on_off == 0x31) {
        // ON -- 0001 0000
        new_output = curr_output | 0x01;
        io_expander_set_output_port(new_output);

    // this should be factored out into servo.c
    } else if (on_off == 0x32) {
        if (servo_degree + SERVO_INC > 180) {
            servo_degree = 180;
        } else {
            servo_degree = servo_degree + SERVO_INC;
        } 
        printf("set servo %d\r\n", servo_degree);
        servo_set(servo_degree);

    } else if (on_off == 0x33) {
        if (servo_degree - SERVO_INC < 0) {
            servo_degree = 0;
        } else {
            servo_degree = servo_degree - SERVO_INC;
        }
        printf("set servo %d\r\n", servo_degree);
        servo_set(servo_degree);
    }
}
