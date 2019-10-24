#include <stdio.h>
#include "platform.h"
#include "xil_types.h"	// Added for integer type definitions
#include "seg7_display.h" // Added for 7-segment definitions
#include "xgpio.h"
#include "gpio_init.h"

int uartReceive();

// Declaration of functions to start running the calculator and determine which button was pressed
void runCalculator();
void readButtons();
void readNumbers();

// Functions declarations for arithmetical operations
void add(u8 firstOperand, u8 secondOperand);
void subtract(u8 firstOperand, u8 secondOperand);
void multiply(u8 firstOperand, u8 secondOperand);
void divide(u8 firstOperand, u8 secondOperand);
void power(u8 firstOperand, u8 secondOperand);
void squareRoot(u8 firstOperand);

// Global variables for inputs
u16 slideSwitchIn = 0;
s8 firstOperand = 0;
s8 secondOperand = 0;
u8 operationType;

// Global variable for outputting the result of operation
s16 result;

// Variables holding the values of push buttons
u8 leftButton;
u8 rightButton;
u8 upButton;
u8 downButton;

int main()
{
	init_platform();
	int status;

	// Initialise the GPIOs
	status = initGpio();
	if (status != XST_SUCCESS)
	{
		print("GPIOs initialisation failed!\n\r");
		cleanup_platform();
		return 0;
	}

	// Setup the Interrupt System
	status = setUpInterruptSystem();
	if (status != XST_SUCCESS)
	{
		print("Interrupt system setup failed!\n\r");
		cleanup_platform();
		return 0;
	}

	print("System initialisation successful!\n\r");

	print("Change addition and subtraction to power and square root?\n\r"
			"Type 'y' or 'n' and press enter.\n\r");

	int enableExtraOps = uartReceive();

	runCalculator(enableExtraOps);

	cleanup_platform();
	return 0;
}

// Function that starts the calculator and keeps it running forever
void runCalculator(int mode)
{
	while (1)
	{
		// Call the method to display number
		displayNumber(result);
		XGpio_DiscreteWrite(&LED_OUT, 1, result);

		readNumbers();
		readButtons();

		// Perform appropriate arithmetic operation according to which button was pressed
		if (upButton)
		{
			// Can change between addition and power (there is not enough buttons to
			//assign all the ops separately)
			if (mode == 'y')
				power(firstOperand, secondOperand);
			else
				add(firstOperand, secondOperand);
		}
		else if (downButton)
		{
			// Can change between subtraction and square root
			if (mode == 'y')
				squareRoot(firstOperand);
			else
				subtract(firstOperand, secondOperand);
		}
		else if (leftButton)
			multiply(firstOperand, secondOperand);
		else if (rightButton)
			divide(firstOperand, secondOperand);
	}
}

void readButtons()
{
	upButton = XGpio_DiscreteRead(&P_BTN_UP, 1);
	downButton = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
	leftButton = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
	rightButton = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
	return;
}

void readNumbers()
{
	// Read the 16 bit number from slide switches
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);

	// Get the first number by taking 8 MSB of slideSwitchIn
	firstOperand = slideSwitchIn >> 8;

	// Get the second number by taking 8 LSB of slideSwitchIn
	secondOperand = slideSwitchIn & 0xFF;
}
