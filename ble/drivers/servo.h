/*
 *  Created on: Oct 2, 2023
 *      Author: Cade Kenyon
 */

#ifndef servo_H__
#define servo_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdio.h>

/* Macros */
#define PIN_MCU_SERVO_PWM P9_2

/* Public Global Variables */
extern cyhal_pwm_t mServo;

/* Public API */
cy_rslt_t servo_init(void);
cy_rslt_t servo_set(int degree);
#endif 
