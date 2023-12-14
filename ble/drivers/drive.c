/*
 * drive.c
 *
 *  Created on: Oct 22, 2023
 *      Author: Cade 
 */

#include "drive.h"

cyhal_pwm_t mDrive;
char bogo_dir;

#define USE_PWM 1
#define USE_GPIO 0

#define DRIVE_DUTY 75
#define DRIVE_FRQ 10000

# if USE_PWM

cyhal_pwm_t mDriveA1;
cyhal_pwm_t mDriveA2;
cyhal_pwm_t mDriveB1;
cyhal_pwm_t mDriveB2;

# endif


cy_rslt_t drive_init(void)
{
    cy_rslt_t rslt;
#if USE_GPIO
    rslt = cyhal_gpio_init(PIN_MCU_DRIVE_A1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    rslt = cyhal_gpio_init(PIN_MCU_DRIVE_A2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    rslt = cyhal_gpio_init(PIN_MCU_DRIVE_B1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    rslt = cyhal_gpio_init(PIN_MCU_DRIVE_B2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    return rslt;
#endif
#if USE_PWM
    rslt = cyhal_pwm_init(&mDriveA1, PIN_MCU_DRIVE_A1, NULL);
    rslt = cyhal_pwm_init(&mDriveA2, PIN_MCU_DRIVE_A2, NULL);
    rslt = cyhal_pwm_init(&mDriveB1, PIN_MCU_DRIVE_B1, NULL);
    rslt = cyhal_pwm_init(&mDriveB2, PIN_MCU_DRIVE_B2, NULL);
    rslt = cyhal_pwm_set_duty_cycle(&mDriveA1, DRIVE_DUTY, DRIVE_FRQ);
    rslt = cyhal_pwm_set_duty_cycle(&mDriveA2, DRIVE_DUTY, DRIVE_FRQ);
    rslt = cyhal_pwm_set_duty_cycle(&mDriveB1, DRIVE_DUTY, DRIVE_FRQ);
    rslt = cyhal_pwm_set_duty_cycle(&mDriveB2, DRIVE_DUTY, DRIVE_FRQ);
    bogo_dir = 'q';
#endif 
}

cy_rslt_t drive_dir(int dir) {
#if USE_GPIO
    cyhal_gpio_write(PIN_MCU_DRIVE_A1, false);
    cyhal_gpio_write(PIN_MCU_DRIVE_A2, false);
    cyhal_gpio_write(PIN_MCU_DRIVE_B1, false);
    cyhal_gpio_write(PIN_MCU_DRIVE_B2, false);
    cyhal_system_delay_ms(200); 
    // backwards
    if (dir == 0x30) {
        cyhal_gpio_write(PIN_MCU_DRIVE_A1, true);
        cyhal_gpio_write(PIN_MCU_DRIVE_A2, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_B1, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_B2, true);
    // forwards 
    } else if (dir == 0x31) {
        cyhal_gpio_write(PIN_MCU_DRIVE_A1, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_A2, true);
        cyhal_gpio_write(PIN_MCU_DRIVE_B1, true);
        cyhal_gpio_write(PIN_MCU_DRIVE_B2, false);
    // left
    } else if (dir == 0x32) {
        cyhal_gpio_write(PIN_MCU_DRIVE_A1, true);
        cyhal_gpio_write(PIN_MCU_DRIVE_A2, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_B1, true);
        cyhal_gpio_write(PIN_MCU_DRIVE_B2, false);
    // right
    } else if (dir == 0x33) {
        cyhal_gpio_write(PIN_MCU_DRIVE_A1, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_A2, true);
        cyhal_gpio_write(PIN_MCU_DRIVE_B1, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_B2, true);
    } else if (dir == 0x34) {
        cyhal_gpio_write(PIN_MCU_DRIVE_A1, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_A2, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_B1, false);
        cyhal_gpio_write(PIN_MCU_DRIVE_B2, false);
    }
#endif
#if USE_PWM
    uint8_t prox_reading = prox_read0();
    cyhal_pwm_stop(&mDriveA1);
    cyhal_pwm_stop(&mDriveA2);
    cyhal_pwm_stop(&mDriveB1);
    cyhal_pwm_stop(&mDriveB2);
    cyhal_system_delay_ms(10); 
    
    uint8_t masked_warning;
    // forward
    if (dir == 0x30) {
        masked_warning = prox_reading & 0x10;
        if (masked_warning != 0x00) {
            cyhal_pwm_start(&mDriveA1);
            cyhal_pwm_start(&mDriveB1);
            bogo_dir = 'w';
        }
    // backwards 
    } else if (dir == 0x31) {
        masked_warning = prox_reading & 0x40;
        if (masked_warning != 0x00) {
            cyhal_pwm_start(&mDriveA2);
            cyhal_pwm_start(&mDriveB2);
            bogo_dir = 's';
        }
    // left
    } else if (dir == 0x32) {
        masked_warning = prox_reading & 0x20;
        if (masked_warning != 0x00) {
            cyhal_pwm_start(&mDriveA1);
            cyhal_pwm_start(&mDriveB2);
            bogo_dir = 'a';
        }
    // right
    } else if (dir == 0x33) {
        masked_warning = prox_reading & 0x80;
        if (masked_warning != 0x00) {
            cyhal_pwm_start(&mDriveA2);
            cyhal_pwm_start(&mDriveB1);
            bogo_dir = 'd';
        }
    } else if (dir == 0x34) {
        cyhal_pwm_stop(&mDriveA1);
        cyhal_pwm_stop(&mDriveB2);
        cyhal_pwm_stop(&mDriveB1);
        cyhal_pwm_stop(&mDriveB2);
        bogo_dir = 'q';
    }
#endif
}
