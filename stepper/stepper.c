/*
 * stepper.c
 *
 *  Created on: 9 Sep. 2021
 *      Author: deven
 */

#include "stepper.h"

uint64_t volatile delay_cycle = 0;

/*********************************************************************
 * @fn      		  - stepper_init
 *
 * @brief             - This function initializes the Stepper Motor peripheral
 *
 * @param[in]         - base address of stepper handle structure pointing to various configuration settings
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  The DWT_CTRL and CCYNT register are also initialized to count the CPU cycles
 */
void stepper_init(Stepper_Handle_t *hstep)
{

	GPIO_InitTypeDef stepport_init;

	//Initialize the DWT_CTRL and CCYNT register
	KIN1_InitCycleCounter();	 /* enable DWT hardware */
	KIN1_ResetCycleCounter(); 	/* reset cycle counter */
	KIN1_EnableCycleCounter(); 	/* start counting */

	//1. GPIO pins configuration used for motor coils
	stepport_init.Mode = GPIO_MODE_OUTPUT_PP;
	stepport_init.Pull = GPIO_NOPULL;
	stepport_init.Speed = GPIO_SPEED_FAST;

	//Motor A IN1 Pin
	stepport_init.Pin = hstep->A_IN1;
	HAL_GPIO_Init(hstep->pGPIO, &stepport_init);

	//Motor A IN2 Pin
	stepport_init.Pin = hstep->A_IN2;
	HAL_GPIO_Init(hstep->pGPIO, &stepport_init);

	//Motor B IN1 Pin
	stepport_init.Pin = hstep->B_IN1;
	HAL_GPIO_Init(hstep->pGPIO, &stepport_init);

	//Motor B IN2 Pin
	stepport_init.Pin = hstep->B_IN2;
	HAL_GPIO_Init(hstep->pGPIO, &stepport_init);
}

/*********************************************************************
 * @fn      		  - step_speedconfig
 *
 * @brief             - This function calculates the CPU cycles required to create a Step delay
 *
 * @param[in]         - base address of stepper handle structure pointing to various configuration settings
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  Returns the number of CPU cycles required to create the delay
 *
 * @Note              -
 */
uint64_t step_speedconfig(Stepper_Handle_t *hstep)
{
	double step_delay = (60 * STEP_ANGLE) / (hstep->speed * FULL_ROTATION_ANGLE);
	return (HAL_RCC_GetHCLKFreq()/(1/step_delay));
}

/*********************************************************************
 * @fn      		  - degree_config
 *
 * @brief             - This function rotates the stepper motor based on angle between 1 to 360 degree
 *
 * @param[in]         - base address of stepper handle structure pointing to various configuration settings
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  Returns the HAL status
 *
 * @Note              - Either use the step_config function or degree_config based on the application
 */
HAL_StatusTypeDef degree_config(Stepper_Handle_t *hstep)
{
	//Calculate degree of the rotation
	hstep->number_of_steps = (hstep->degree / STEP_ANGLE);

	if(step_config(hstep) != HAL_OK)
		return HAL_ERROR;
	else
		return HAL_OK;
}

/*********************************************************************
 * @fn      		  - step_config
 *
 * @brief             - This function rotates the stepper motor for a specific number of steps.
 *
 * @param[in]         - base address of stepper handle structure pointing to various configuration settings
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  Returns the HAL status
 *
 * @Note              - Either use the step_config function or degree_config based on the application
 */
HAL_StatusTypeDef step_config(Stepper_Handle_t *hstep)
{
	uint8_t volatile step_count = hstep->number_of_steps;
	uint8_t volatile Nstep = 0;
	uint64_t volatile present_cycle = 0;
	uint64_t step_cycles = step_speedconfig(hstep);

	if(step_cycles >= MIN_SPEED_DELAY && step_cycles <= MAX_SPEED_DELAY)
	{
		present_cycle = KIN1_GetCycleCounter();
		delay_cycle = present_cycle;

		while(step_count > 0)
		{
			present_cycle = KIN1_GetCycleCounter();

			if((present_cycle - delay_cycle) >= step_cycles)
			{
				printf("Cycles : %lu\n", (long unsigned int)present_cycle);
				delay_cycle = present_cycle;
				if(hstep->direction == CW)
				{
					Nstep++;
					if(Nstep == hstep->number_of_steps)
					{
						Nstep = 0;
					}
				}
				else if (hstep->direction == CCW)
				{
					if(Nstep == 0)
					{
						Nstep = hstep->number_of_steps;
					}
					Nstep--;
				}
				if(Nstep % 4 == 0)
				{
					printf("1\n");
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN1, GPIO_PIN_SET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN2, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_SET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_RESET);
				}
				else if(Nstep % 4 == 1)
				{
					printf("2\n");
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_SET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_RESET);
				}
				else if (Nstep % 4 == 2)
				{
					printf("3\n");
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_SET);
				}
				else if (Nstep % 4 == 3)
				{
					printf("4\n");
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN1, GPIO_PIN_SET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->A_IN2, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(hstep->pGPIO, hstep->B_IN1, GPIO_PIN_SET);
				}
				step_count --;
			}
		}
		KIN1_DisableCycleCounter(); /* disable counting if not used any more */
		return HAL_OK;
	}
	else
	{
		KIN1_DisableCycleCounter(); /* disable counting if not used any more */
		return HAL_ERROR;
	}
}
