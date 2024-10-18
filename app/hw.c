/*
 * hw.c
 *
 *  Created on: Oct 14, 2024
 *      Author: eufil
 */


#include "hw.h"
#include "main.h"

uint32_t tempo_em_mili()
{
	return HAL_GetTick();
}
