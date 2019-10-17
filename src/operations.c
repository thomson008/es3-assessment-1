#include "xil_types.h"
#include "xgpio.h"
#include "gpio_init.h"
#include "seg7_display.h"
#include <math.h>

// Global variables from main.c
extern s16 result;
extern s8 firstOperand;
extern s8 secondOperand;

// Also from main.c, representing the values of 4 push buttons
extern u8 leftButton;
extern u8 rightButton;
extern u8 upButton;
extern u8 downButton;

void add(s8 a, s8 b)
{
	while(upButton)
	{
		upButton = XGpio_DiscreteRead(&P_BTN_UP, 1);
		displayNumber(result);
	}

	result =  a + b;
}

void subtract(s8 a, s8 b)
{
	while(downButton)
	{
		downButton = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
		displayNumber(result);
	}

	result = a - b;
}

void multiply(s8 a, s8 b)
{
	while(leftButton)
	{
		leftButton = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		displayNumber(result);
	}

	result = a * b;
}

void divide(s8 a, s8 b)
{
	while(rightButton)
	{
		rightButton = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		displayNumber(result);
	}

	result = a / b;
}

void power(s8 a, s8 b)
{
	while(upButton)
	{
		upButton = XGpio_DiscreteRead(&P_BTN_UP, 1);
		displayNumber(result);
	}

	result = pow(a, b);
}

void squareRoot(s8 a)
{
	while(downButton)
	{
		downButton = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
		displayNumber(result);
	}

	result = sqrt(a);
}
