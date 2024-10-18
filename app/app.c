/*
 * app.c
 *
 *  Created on: Oct 14, 2024
 *      Author: eufil
 */
#include "main.h"
#include "app.h"
#include "hw.h"

uint32_t tempo_restante_p1 = TEMPO_TOTAL;
uint32_t tempo_restante_p2 = TEMPO_TOTAL;

void app_init(void){

}


void app_loop(void){
	//retorna struct com os valores das leituras
	ESTADOS_DOS_PINOS estado_atual = checa_estados_pinos();

	//retorna o valor interpretado das leituras
	ESTADOS_MAQUINA estado = troca_de_estado(estado_atual);

	HAL_Delay(100);
	//debounce
	if(troca_de_estado(checa_estados_pinos()) == estado_atual){
		switch(estado_atual){
			case PAUSE:
				toca_buzzer(FREQUENCIA_DE_PAUSE);
				continue;
			case TURNO_P1:
				cotinue;
			case TURNO_P2:
				continue;
		}
	}


	//=============================================================================================================
	/Código para gerenciar o i2c do display aqui/










	//==============================================================================================================


}


void toca_buzzer(float frequencia_de_toque) {
    // Calculo do valor do periodo
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq(); // Frequencia do clock do timer
    uint32_t prescaler = TIM2->PSC + 1;            // Pegue o valor do prescaler do timer

    uint32_t periodo = (timer_clock / (prescaler * frequencia_de_toque)) - 1;

    // Ajuste do período do timer para a frequência desejada
    __HAL_TIM_SET_AUTORELOAD(&htim2, periodo);

    // Ajuste do duty cycle para 50%
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, periodo / 2);

    // Iniciando o PWM
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void para_buzzer() {
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);  // Para o toque do buzzer, visto que precisamos só de um toque para indica os eventos
}


ESTADOS_MAQUINA troca_de_estado(ESTADOS_DOS_PINOS estado_atual){
	if(estado_atual.ESTADO_PINO_TROCA_PARA_P1 == GPIO_PIN_SET){
			//código para gerenciar troca para player 1
			return TURNO_P1; //retorna o tempo em milisegundos que o processador está ligado

		}
		else if(estado_atual.ESTADO_PINO_TROCA_PARA_P2 == GPIO_PIN_SET){
			//código para gerenciar troca para player 2
			return TURNO_P2; //retorna o tempo em milisegundos que o processador está ligado

		}
		else if(estado_atual.ESTADO_PINO_TROCA_PARA_P1 == GPIO_PIN_SET || estado_atual.ESTADO_PINO_TROCA_PARA_P2 == GPIO_PIN_SET){
			//código para gerenciar o pause
			return PAUSE; //retorna o tempo em milisegundos que o processador está ligado

		}
}

ESTADOS_DOS_PINOS checa_estados_pinos()
{
	ESTADOS_DOS_PINOS estado_atual;
	estado_atual.ESTADO_PINO_TROCA_PARA_P2 = HAL_GPIO_ReadPin(Pino_Troca_P2_GPIO_Port, Pino_Troca_P2_Pin);
	estado_atual.ESTADO_PINO_TROCA_PARA_P1 = HAL_GPIO_ReadPin(Pino_Troca_P1_GPIO_Port, Pino_Troca_P1_Pin);
	return estado_atual;
}
