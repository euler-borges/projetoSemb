/*
 * app.c
 *
 *  Created on: Oct 14, 2024
 *      Author: eufil
 */
#include "main.h"
#include "app.h"
#include "hw.h"



void app_init(void){

}


void app_loop(void){
	uint32_t timing = tempo_em_mili(); //retorna o tempo em milisegundos que o processador está ligado
	GPIO_PinState estado_pino_troca_para_p2 = HAL_GPIO_ReadPin(Pino_Troca_P2_GPIO_Port, Pino_Troca_P2_Pin);
	GPIO_PinState estado_pino_troca_para_p1 = HAL_GPIO_ReadPin(Pino_Troca_P1_GPIO_Port, Pino_Troca_P1_Pin);


	if(estado_pino_troca_para_p1 == GPIO_PIN_SET){
		//código para gerenciar troca para player 1
	}
	else if(estado_pino_troca_para_p2 == GPIO_PIN_SET){
		//código para gerenciar troca para player 2
	}
	else if(estado_pino_troca_para_p2 == GPIO_PIN_SET || estado_pino_troca_para_p1 == GPIO_PIN_SET){
		//código para gerenciar o pause
	}

	//=============================================================================================================
	/*Código para gerenciar o i2c do display aqui*/










	//==============================================================================================================


}


void toca_buzzer(float frequencia_de_toque)
{

}
