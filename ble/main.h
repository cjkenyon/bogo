/*
 * main.h
 *
 *  Created on: Aug 30, 2022
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_


#define ECE453_USR_BTN P5_6
#define ECE453_USR_LED P5_5

#define ECE453_DEBUG_TX	P6_5
#define ECE453_DEBUG_RX P6_4

#include "drivers/drive.h"
#include "drivers/i2c.h"
#include "drivers/io.h"
#include "drivers/pump.h"
#include "drivers/servo.h"
#include "drivers/buzzer.h"
#include "drivers/lights.h"
#include "drivers/LM75.h"
#include "drivers/prox.h"

#endif /* MAIN_H_ */
