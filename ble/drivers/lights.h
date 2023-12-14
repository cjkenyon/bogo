#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "i2c.h"
#include "main.h"

/* Macros */
#define PIN_MCU_Lights_PWM P10_2

/* Public Global Variables */
extern cyhal_pwm_t mLights;

/**
 * Turn the lights on and off
*/
void lights_init();
void lights(bool enable);
