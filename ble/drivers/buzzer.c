/*
 * buzzer.c
 *
 *  Created on: Nov, 2023
 *      Author: Cade 
 */

#include "buzzer.h"

/**
 * Turn the buzzer on and off
*/

cyhal_pwm_t mBuzzer;

void buzz_init() {
    cy_rslt_t   rslt;

    rslt = cyhal_pwm_init(&mBuzzer, PIN_MCU_BUZZER_PWM, NULL);
    if (rslt == CY_RSLT_SUCCESS)
    {
        printf("buzzer init complte\r\n");
    	rslt = cyhal_pwm_set_duty_cycle(&mBuzzer, 50.0, 1000);
    }
}

void buzz(bool enable) {

    if (enable) {
        cyhal_pwm_start(&mBuzzer);

    } else {
        cyhal_pwm_stop(&mBuzzer);

    }
}
