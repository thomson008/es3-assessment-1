/*
 * operations.c
 *
 *  Created on: 16 Oct 2019
 *      Author: Tomek
 */
#include "xil_types.h"
#include "xgpio.h"
#include "gpio_init.h"
#include "seg7_display.h"
#include <math.h>

s16 result = 0;

s16 add(u8 upButton, s8 firstOperand, s8 secondOperand)
{
	while(upButton)
	{
		upButton = XGpio_DiscreteRead(&P_BTN_UP, 1);
		displayNumber(result);
	}

	result =  firstOperand + secondOperand;
	return result;
}

s16 subtract(u8 downButton, s8 firstOperand, s8 secondOperand)
{
	while(downButton)
	{
		downButton = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
		displayNumber(result);
	}

	result = firstOperand - secondOperand;
	return result;
}

s16 multiply(u8 leftButton, s8 firstOperand, s8 secondOperand)
{
	while(leftButton)
	{
		leftButton = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		displayNumber(result);
	}

	result = firstOperand * secondOperand;
	return result;
}

s16 divide(u8 rightButton, s8 firstOperand, s8 secondOperand)
{
	while(rightButton)
	{
		rightButton = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		displayNumber(result);
	}

	result = firstOperand / secondOperand;
	return result;
}

s16 power(u8 upButton, s8 firstOperand, s8 secondOperand)
{
	while(upButton)
	{
		upButton = XGpio_DiscreteRead(&P_BTN_UP, 1);
		displayNumber(result);
	}

	result = pow(firstOperand, secondOperand);
	return result;
}

s16 squareRoot(u8 downButton, s8 firstOperand, s8 secondOperand)
{
	while(downButton)
	{
		downButton = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
		displayNumber(result);
	}

	result = sqrt(firstOperand);
	return result;
}
