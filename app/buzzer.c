/*
 * buzzer.c
 *
 *  Created on: Oct 24, 2024
 *      Author: eufil
 */


#include <stdint.h>
#include "buzzer.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;


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
