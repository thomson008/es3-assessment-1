#include <stdio.h>
#include "xil_types.h"		// Added for integer type definitions
#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"

u8 digitDisplayed = FALSE;
u8 digits[4];
u8 numOfDigits;
u8 digitToDisplay;
u8 digitNumber;

void displayNumber(s16 number)
{
	u8 count;
	/* Note that 9999 is the maximum number that can be displayed
	 * Therefore, check if the number is less than or equal to 9999
	 * and display the number otherwise, display dashes in all the four segments
	 */
	if (number <= 9999 && number > -999)
	{
		// Call the calculateDigits method to determine the digits of the number
		calculateDigits(number);
		/* Do not display leading zeros in a number,
		 * but if the entire number is a zero, it should be displayed.
		 * By displaying the number from the last digit, it is easier
		 * to avoid displaying leading zeros by using the numOfDigits variable
		 */
		count = 4;
		while (count > 4 - numOfDigits)
		{
			digitToDisplay = digits[count-1];
			digitNumber = count;
			count--;
			/* Wait for timer interrupt to occur and ISR to finish
			 * executing digit display instructions
			 */
			while (digitDisplayed == FALSE);
			digitDisplayed = FALSE;
		}
	}
	else
	{

		// << Write the code to display dashes for out-of-range numbers >>
		count = 1;

		while (count < 5)
		{
			digitToDisplay = NUMBER_DASH;
			digitNumber = count;
			count++;


			while(!digitDisplayed);
			digitDisplayed = FALSE;
		}

	}
}

void calculateDigits(s16 number)
{
	u8 fourthDigit;
	u8 thirdDigit;
	u8 secondDigit;
	u8 firstDigit;

	// Added handling for negative numbers
	if (number < -99) {
		numOfDigits = 4;

		fourthDigit = - number % 10;
		thirdDigit = - (number / 10) % 10;
		secondDigit = - number / 100;
		firstDigit = 11;
	}

	else if (number < -9)
	{
		numOfDigits = 3;

		fourthDigit = - number % 10;
		thirdDigit = - number / 10;
		secondDigit = 11;
		firstDigit = 0;
	}

	else if (number < 0)
	{
		numOfDigits = 2;

		fourthDigit = - number;
		thirdDigit = 11;
		secondDigit = 0;
		firstDigit = 0;
	}

	// Check if number is up to four digits
	else if (number > 999)
	{
		numOfDigits = 4;

		fourthDigit  = number % 10;
		thirdDigit = (number / 10) % 10;
		secondDigit  = (number / 100) % 10;
		firstDigit = number / 1000;
	}
	// Check if number is three-digits long
	else if (number > 99 && number < 1000)
	{
		numOfDigits = 3;

		fourthDigit  = number % 10;
		thirdDigit = (number / 10) % 10;
		secondDigit  = (number / 100) % 10;
		firstDigit = 0;
	}
	// Check if number is two-digits long
	else if (number > 9 && number < 100)
	{
		numOfDigits = 2;

		// << Write code to assign digits here >>
		fourthDigit = number % 10;
		thirdDigit = number / 10;
		secondDigit = 0;
		firstDigit = 0;
	}
	// Check if number is one-digit long
	else if (number >= 0 && number < 10)
	{
		numOfDigits = 1;

		// << Write code to assign digits here >>
		fourthDigit = number;
		thirdDigit = 0;
		secondDigit = 0;
		firstDigit = 0;

	}

	digits[0] = firstDigit;
	digits[1] = secondDigit;
	digits[2] = thirdDigit;
	digits[3] = fourthDigit;

	return;
}

void displayDigit()
{
	/*
	 * The timer ISR is used to call this function to display the digits
	 */

	// Write the appropriate value to the SEG7_HEX_OUT object
	switch (digitToDisplay)
	{
		case NUMBER_BLANK :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_BLANK);
			break;
		case 0 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ZERO);
			break;
		case 1 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ONE);
			break;

		// << Complete the case statement for other digit values >>
		case 2:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_TWO);
			break;

		case 3:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_THREE);
			break;

		case 4:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FOUR);
			break;

		case 5:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FIVE);
			break;
			
		case 6:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SIX);
			break;

		case 7:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SEVEN);
			break;

		case 8:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_EIGHT);
			break;

		case 9:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_NINE);
			break;

		case NUMBER_DASH:
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_DASH);
			break;


	}

	// Select the appropriate digit
	if (digitNumber == 1) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FIRST_SEG);
	}
	else if (digitNumber == 2) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_SECOND_SEG);
	}
	else if (digitNumber == 3) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_THIRD_SEG);
	}
	else if (digitNumber == 4) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FOURTH_SEG);
	}

	digitDisplayed = TRUE;
	return;
}
