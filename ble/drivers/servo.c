/*
 * servo.c
 *
 *  Created on: Oct 2, 2023
 *      Author: Cade 
 */

#include "servo.h"

// min operating frequency is 50
// angle is decided by sending a pulse between
// 500 and 2500 us

#define SERVO_MIN 2.5
#define SERVO_FRQ 50
#define SERVO_CENTER 7.5

cyhal_pwm_t mServo;

cy_rslt_t servo_init(void)
{
	cy_rslt_t   rslt;

    rslt = cyhal_pwm_init(&mServo, PIN_MCU_SERVO_PWM, NULL);
    if (rslt == CY_RSLT_SUCCESS)
    {
        // Set the data rate to 1 Mbps
        printf("servo init complte\r\n");
    	rslt = cyhal_pwm_set_duty_cycle(&mServo, SERVO_CENTER, SERVO_FRQ);
    }
    if (rslt == CY_RSLT_SUCCESS) {
        printf("servo set complete\r\n");
        cyhal_pwm_start(&mServo);
    }
    printf("we are about to return\r\n");

	return rslt;

}

cy_rslt_t servo_set(int degree) {
    cy_rslt_t rslt;
    float duty_cycle = ((degree / 180.0) * 10) + SERVO_MIN;
    rslt = cyhal_pwm_set_duty_cycle(&mServo, duty_cycle, SERVO_FRQ);
    if (rslt == CY_RSLT_SUCCESS) {
        cyhal_pwm_start(&mServo);
        printf("servo set: %d\n", degree);
    }
    return rslt;
}

