/*
 * portsMapping.c
 *
 *  Created on: Dec 1, 2019
 *      Author: open-silicon
 */

#include "portsMapping.h"
#include "sapi.h"

const uint8_t gpioPortsMapping[9][5] = {
		{6, 1, 3, 0, SCU_MODE_FUNC0},
		{6, 4, 3, 3, SCU_MODE_FUNC0},
		{6, 5, 3, 4, SCU_MODE_FUNC0},
		{6, 7, 5, 15, SCU_MODE_FUNC4},
		{6, 8, 5, 16, SCU_MODE_FUNC4},
		{6, 9, 3, 5, SCU_MODE_FUNC0},
		{6, 10, 3, 6, SCU_MODE_FUNC0},
		{6, 11, 3, 7, SCU_MODE_FUNC0},
		{6, 12, 3, 8, SCU_MODE_FUNC0},
};
