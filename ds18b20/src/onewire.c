/* Copyright 2015, Pablo Ridolfi
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** @brief Brief for this file.
 **
 **/

/** \addtogroup groupName Group Name
 ** @{ */

/*==================[inclusions]=============================================*/

#include "sapi.h"

/*==================[macros and definitions]=================================*/

#define owREAD()	Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 0)
#define owLOW()		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 3, 0)
#define owHIGH()	Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 3, 0)

/*==================[internal data declaration]==============================*/

static volatile uint32_t * DWT_CTRL = (uint32_t *)0xE0001000;
static volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static uint8_t owCrc(uint8_t code[], uint8_t n)
{
	uint8_t i, j, a, b, cy=0, crc=0;

	for(i=0; i<n; i++)
	{
		a = code[i];
		b = code[i];
		for(j=0; j<8; j++)
		{
			a ^= crc;
			cy = a&1;
			a = crc;
			if(cy)
				a ^= 0x18;

			if(cy)
			{
				cy = a&1;
				a >>= 1;
				a |= 0x80;
			}
			else
			{
				cy = a&1;
				a >>= 1;
			}
			crc = a;

			if(b&1)
			{
				b >>= 1;
				b |= 0x80;
			}
			else
			{
				b >>= 1;
			}
			a = b;
		}
	}
	return crc;
}

static int owPresence(void)
{
    delayUs(1000);

	owOUT();
	owLOW();
	delayUs(480);
	owIN();
	delayUs(40);

	if(owREAD()==true)
	{
		return -1;
	}
	else
	{
		while(owREAD()==false);
		return 0;
	}
}

static void owCmd(uint8_t cmd, void * buffer, uint8_t n)
{
	uint8_t i = 1, j;
	uint8_t * p = (uint8_t *)buffer;

	owOUT();
	do
	{
		if(cmd & i)
		{
			owLOW();
			delayUs(3);
			owHIGH();
			delayUs(60);
		}
		else
		{
			owLOW();
			delayUs(60);
			owHIGH();
			delayUs(10);
		}
		if(i==0x80)
		{
			break;
		}
		else
		{
			i <<= 1;
		}
	}while(i != 0);

	for(i=0; i<n; i++)
	{
		p[i] = 0;
		for(j=0; j<8; j++)
		{
			owOUT();
			owLOW();
			delayUs(3);
			owIN();
			delayUs(12);
			p[i] >>= 1;
			if(owREAD()) p[i] |= 0x80;
			delayUs(55);
		}
	}
}

void owIN(){
	Chip_SCU_PinMux(
	            6,
	            1,
	            SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,
				SCU_MODE_FUNC0
	         );
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 3, 0);
}

void owOUT(){
	Chip_SCU_PinMux(
	            6,
	            1,
	            SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
				SCU_MODE_FUNC0
	         );
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);
}

/*==================[external functions definition]==========================*/

void owInit(void)
{
	/* Init cycle counter */
    *DWT_CTRL |= 1;

    /* Init pin P0.20 */
    Chip_SCU_PinMux(6, 1,
    		SCU_MODE_PULLUP,
			SCU_MODE_FUNC0);

    owIN();
}

int owReadROM(void * buffer8)
{
	int rv = -1;
	uint8_t crc = 0;
	uint8_t * p = (uint8_t *)buffer8;

	if(owPresence()==0)
	{
		delayUs(400);

		__set_PRIMASK(1);

		owCmd(0x33, p, 8); /* READ ROM CMD */

		__set_PRIMASK(0);

		crc = owCrc(p, 7);

		if(crc == p[7])
		{
			rv = 0;
		}
	}
	return rv;
}

int owReadScratch(void * buffer9)
{
	int rv = -1;
	uint8_t crc = 0;
	uint8_t * p = (uint8_t *)buffer9;

	if(owPresence()==0)
	{
		delayUs(400);

		__set_PRIMASK(1);

		owCmd(0x33, p, 8); /* READ ROM CMD */

		owCmd(0xBE, p, 9);

		__set_PRIMASK(0);

		crc = owCrc(p, 8);

		if(crc == p[8])
		{
			rv = 0;
		}
	}
	return rv;
}

int owReadTemperature(void)
{
	int rv = -1;
	uint8_t crc = 0;
	uint8_t p[9];

	if(owPresence()==0)
	{
		delayUs(400);

		__set_PRIMASK(1);

		owCmd(0x33, p, 8); /* READ ROM CMD */

		owCmd(0x44, p, 0); /* START CONVERSION */

		__set_PRIMASK(0);

		owIN();
		while(owREAD() == false); /* wait for end of conv */

		owPresence();

		delayUs(400);

		__set_PRIMASK(1);

		owCmd(0x33, p, 8); /* READ ROM CMD */

		owCmd(0xBE, p, 9); /* READ SCRATCH */

		__set_PRIMASK(0);

		crc = owCrc(p, 8);

		if(crc == p[8])
		{
			rv = p[1];
			rv <<= 8;
			rv |= p[0];
		}
	}
	return rv;
}

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
