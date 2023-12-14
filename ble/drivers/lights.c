#include "lights.h"

cyhal_pwm_t mLights;

void lights_init(){

    cyhal_pwm_init(&mLights, PIN_MCU_Lights_PWM, NULL);
    cyhal_pwm_set_duty_cycle(&mLights, 50, 5);

}

/**
 * Turn the lights on and off
*/
void lights(bool enable) {

    if (enable) {
        cyhal_pwm_start(&mLights);

    }
    else {
        cyhal_pwm_stop(&mLights);
        
    }
}
