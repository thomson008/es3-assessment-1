/******************************************************************************
*
* Copyright (C) 2002 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
/******************************************************************************/
/**
*
* @file xuartlite_polled_example.c
*
* This file contains a design example using the UartLite driver (XUartLite) and
* hardware device using the polled mode.
*
* @note
*
* The user must provide a physical loopback such that data which is
* transmitted will be received.
*
* MODIFICATION HISTORY:
* <pre>
* Ver   Who  Date	 Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a jhl  02/13/02 First release
* 1.00a sv   06/13/05 Minor changes to comply to Doxygen and coding guidelines
* 2.00a ktn  10/20/09 Updated this example to wait for valid data in receive
*		      fifo instead of Tx fifo empty to update receive buffer
*		      and minor changes as per coding guidelines.
* </pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xstatus.h"
#include "xuartlite.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define UARTLITE_DEVICE_ID	XPAR_UARTLITE_0_DEVICE_ID

/*
 * The following constant controls the length of the buffers to be sent
 * and received with the UartLite, this constant must be 16 bytes or less since
 * this is a single threaded non-interrupt driven example such that the
 * entire buffer will fit into the transmit and receive FIFOs of the UartLite.
 */
#define TEST_BUFFER_SIZE 1

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

//int UartLiteReceive();

/************************** Variable Definitions *****************************/

XUartLite UartLite;		/* Instance of the UartLite Device */

/*
 * The following buffers are used in this example to send and receive data
 * with the UartLite.
 */
//u8 SendBuffer[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */
u8 RecvBuffer[TEST_BUFFER_SIZE];	/* Buffer for Receiving Data */

u8 uart_is_started = FALSE;

/****************************************************************************/
/**
* This function does a minimal test on the UartLite device and driver as a
* design example. The purpose of this function is to illustrate
* how to use the XUartLite component.
*
* This function sends data and expects to receive the data thru the UartLite
* such that a  physical loopback must be done with the transmit and receive
* signals of the UartLite.
*
* This function polls the UartLite and does not require the use of interrupts.
*
* @param	DeviceId is the Device ID of the UartLite and is the
*		XPAR_<uartlite_instance>_DEVICE_ID value from xparameters.h.
*
* @return	XST_SUCCESS if successful, XST_FAILURE if unsuccessful.
*
*
* @note
*
* This function calls the UartLite driver functions in a blocking mode such that
* if the transmit data does not loopback to the receive, this function may
* not return.
*
****************************************************************************/
int uartReceive()
{
	int Status;
	unsigned int ReceivedCount = 0;
	int Index;

	/*
	 * Initialize the UartLite driver so that it is ready to use.
	 */
	if (uart_is_started == FALSE)
	{
		Status = XUartLite_Initialize(&UartLite, UARTLITE_DEVICE_ID);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
		uart_is_started = TRUE;
	}

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		RecvBuffer[Index] = 0;
	}

	/*
	 * Receive the number of bytes which is transfered.
	 * Data may be received in fifo with some delay hence we continuously
	 * check the receive fifo for valid data and update the receive buffer
	 * accordingly.
	 */
	while (1) {
		ReceivedCount += XUartLite_Recv(&UartLite,
					   RecvBuffer + ReceivedCount,
					   TEST_BUFFER_SIZE - ReceivedCount);
		if (ReceivedCount == TEST_BUFFER_SIZE) {
			if (RecvBuffer[0] == 10 || RecvBuffer[0] == 13)
			{
				ReceivedCount = 0;
			}
			else
			{
				return RecvBuffer[0];
			}
		}
	}

	uart_is_started = FALSE;

	return XST_SUCCESS;
}
