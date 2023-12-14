/*
 * buzzer.h
 *
 *  Created on: Nov, 2023
 *      Author: Cade 
 */

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "i2c.h"
#include "main.h"

#define PIN_MCU_BUZZER_PWM P9_0

extern cyhal_pwm_t mBuzzer;

/**
 * Turn the buzzer on and off
*/
void buzz_init();
void buzz(bool enable);
