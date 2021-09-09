/*
 * stepper.h
 *
 *  Created on: 9 Sep. 2021
 *      Author: deven
 */

#ifndef STEPPER_H_
#define STEPPER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* Private defines -----------------------------------------------------------*/
// Stepper_Direction
#define CW	0x00000000U
#define CCW	0x00000001U

//Degree per step
#define STEP_ANGLE 				1.8
#define FULL_ROTATION_ANGLE		360

//Speed delay constrains
#define MIN_SPEED_DELAY		25200U
#define MAX_SPEED_DELAY		25200000U

typedef struct
{
	GPIO_TypeDef *pGPIO;		//GPIO port used
	uint16_t A_IN1;				//IN1 Pin for motor Coil A
	uint16_t A_IN2;				//IN2 Pin for motor Coil A
	uint16_t B_IN1;				//IN1 Pin for motor Coil B
	uint16_t B_IN2;				//IN2 Pin for motor Coil B
	int32_t number_of_steps;	//Number of steps
	uint8_t direction;			//Direction of rotation @ref Stepper_Direction
	uint16_t speed;				//Speed in RPM
	uint16_t degree;			//Turn the stepper motor based on degree value must between 0 to 360
}Stepper_Handle_t;


/*
 * *********************************DWT Control register****************************************
 * */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))

/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)

/*!< DWT Cycle Counter register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))

/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))

/*!< Trace enable bit in DEMCR register */
#define KIN1_TRCENA_BIT              (1UL<<24)

/*
 * *********************************Initialization functions for DWT Control register****************************************
 * */
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */
#define KIN1_InitCycleCounter() 	KIN1_DEMCR |= KIN1_TRCENA_BIT

/*!< Reset cycle counter */
#define KIN1_ResetCycleCounter() 	KIN1_DWT_CYCCNT = 0

/*!< Enable cycle counter */
#define KIN1_EnableCycleCounter() 	KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT

/*!< Disable cycle counter */
#define KIN1_DisableCycleCounter() 	KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT

/*!< Read cycle counter register */
#define KIN1_GetCycleCounter() 		KIN1_DWT_CYCCNT

/* ********************************************Exported functions prototypes ---------------------------------------------*/
void stepper_init(Stepper_Handle_t *hstep);
HAL_StatusTypeDef step_config(Stepper_Handle_t *hstep);
uint64_t step_speedconfig(Stepper_Handle_t *hstep);
HAL_StatusTypeDef degree_config(Stepper_Handle_t *hstep);


#endif /* STEPPER_H_ */
