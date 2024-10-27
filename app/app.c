/*
 * app.c
 *
 *  Created on: Oct 14, 2024
 *      Author: eufil
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "main.h"
#include "app.h"
#include "hw.h"
#include "buzzer.h"
#include "lcd.h"

//variaveis auxiliares
uint32_t tempo_atual = 0;
uint32_t tempo_anterior = 0;
bool buzzer_ja_tocado = false;

ESTADOS_MAQUINA estado_proximo_da_maquina = PAUSE;
ESTADOS_MAQUINA estado_anterior_da_maquina = PAUSE;


extern char* player_1_win;
extern char* player_2_win;
extern char* game_over;


extern volatile uint8_t tens_minutes_counter_player_1;
extern volatile uint8_t tens_seconds_counter_player_1;
extern volatile uint8_t units_minutes_counter_player_1;
extern volatile uint8_t units_seconds_counter_player_1;

extern volatile uint8_t tens_minutes_counter_player_2;
extern volatile uint8_t tens_seconds_counter_player_2;
extern volatile uint8_t units_minutes_counter_player_2;
extern volatile uint8_t units_seconds_counter_player_2;

volatile uint32_t tempo_atual_turno, tempo_anterior_turno, dt;




//feito para usar o htim2 declarado no main.c


void app_init(void){
	handle_start();
}


void app_loop(void){
	//em caso de não acntecer nada, retorna ao qe estava antes
	estado_anterior_da_maquina = estado_proximo_da_maquina;

	//analisa onde se deve ir
	estado_proximo_da_maquina = checa_com_debounce();

	//para o caso de nenhuma acao ser tomada
	if(estado_proximo_da_maquina == NADA){
		estado_proximo_da_maquina = estado_anterior_da_maquina;
	}
	//para o proximo loop

	switch(estado_proximo_da_maquina){
				//cuida do pause do relógio
				case PAUSE:

					pause_lcd();


					//executa um toque do buzzer caso ele ainda não tenha sido tocado
					if(!buzzer_ja_tocado){
						toca_buzzer(FREQUENCIA_DE_PAUSE);
						HAL_Delay(500);
						para_buzzer();
						//mantem registro do buzzer ter sido tocado ou não
						buzzer_ja_tocado = true;
					}



					//mantem registro para troca de turno
					tempo_anterior = tempo_em_mili();

				//cuida de quando esta co rrendo o tempo do jogador 1
				case TURNO_P1:
					//reseta a variavel para o caso de entrar em pause novamente
					buzzer_ja_tocado = false;

					if(estado_anterior_da_maquina == PAUSE){reload_lcd();} //reinicia lcd após despausar

					//salva o momento atual
					tempo_atual = tempo_em_mili();
					//calcula o novo tempo restante do player 1


					/*
					 * Printar o estado do jogo na tela na tela
					 *
					 */
					dt = tempo_atual - tempo_anterior;

					if(dt >= 1000)
					{
						decrease_one_second(estado_proximo_da_maquina);
							if((tens_minutes_counter_player_1 == 0) &&
								(tens_seconds_counter_player_1 == 0) &&
								(units_minutes_counter_player_1 == 0) &&
								(units_seconds_counter_player_1 == 0))
							{
								GAME_OVER(estado_proximo_da_maquina);
								reset_game();
							}
							//mudei pois ocorreria perca de valoress
							tempo_anterior +=1000;
					}



					/*
					 * Fim do print
					 */


					//





				case TURNO_P2:
					//reseta a variavel para o caso de entrar em pause novamente
					buzzer_ja_tocado = false;


					if(estado_anterior_da_maquina == PAUSE){reload_lcd();}

					//salva o momento atual
					tempo_atual = tempo_em_mili();

					dt = tempo_atual - tempo_anterior;

					if(dt >= 1000)
					{
						decrease_one_second(estado_proximo_da_maquina);
						if((tens_minutes_counter_player_2 == 0) &&
								(tens_seconds_counter_player_2 == 0) &&
								(units_minutes_counter_player_2 == 0) &&
								(units_seconds_counter_player_2 == 0))
						{
							GAME_OVER(estado_proximo_da_maquina);
							reset_game();

						}

						//mudei pois ocorreria perca de valoress
						tempo_anterior +=1000;
					}

					/*
					* Printar o estado do jogo na tela na tela
					*/


				//nunca chega aqui, só pra tirar o warning
				case NADA:
	}//fim do switch case

	estado_anterior_da_maquina = estado_proximo_da_maquina;

	//=============================================================================================================
	/*Código para gerenciar o i2c do display aqui*/










	//==============================================================================================================


}//fim do app_loop

void handle_start(){
	/*
	 * Printar a tela de inicio
	 */

	while(checa_com_debounce() != TURNO_P1 || checa_com_debounce() != TURNO_P2){
		continue;
	}

}




ESTADOS_MAQUINA troca_de_estado(ESTADOS_DOS_PINOS estado_atual){
	if(estado_atual.ESTADO_PINO_TROCA_PARA_P1 == GPIO_PIN_SET && estado_atual.ESTADO_PINO_TROCA_PARA_P2 == GPIO_PIN_SET){
			//código para pausar
			return PAUSE;

		}
		else if(estado_atual.ESTADO_PINO_TROCA_PARA_P2 == GPIO_PIN_SET){
			//código para gerenciar troca para player 2
			return TURNO_P2;

		}
		else if(estado_atual.ESTADO_PINO_TROCA_PARA_P1 == GPIO_PIN_SET ){
			//código para gerenciar trocapara player 1
			return TURNO_P1;
		}
		else{
			return NADA;
		}
}

ESTADOS_DOS_PINOS checa_estados_pinos()
{
	ESTADOS_DOS_PINOS estado_atual;
	estado_atual.ESTADO_PINO_TROCA_PARA_P2 = HAL_GPIO_ReadPin(Pino_Troca_P2_GPIO_Port, Pino_Troca_P2_Pin);
	estado_atual.ESTADO_PINO_TROCA_PARA_P1 = HAL_GPIO_ReadPin(Pino_Troca_P1_GPIO_Port, Pino_Troca_P1_Pin);
	return estado_atual;
}


ESTADOS_MAQUINA checa_com_debounce(){
	//retorna struct com os valores das leituras
	ESTADOS_DOS_PINOS estado_atual = checa_estados_pinos();


	//debounce
	HAL_Delay(100);

	ESTADOS_DOS_PINOS estado_proximo = checa_estados_pinos();

	if(estado_proximo.ESTADO_PINO_TROCA_PARA_P1 == estado_atual.ESTADO_PINO_TROCA_PARA_P1 && estado_proximo.ESTADO_PINO_TROCA_PARA_P2 == estado_atual.ESTADO_PINO_TROCA_PARA_P2){
		return troca_de_estado(estado_atual);
		}

	return NADA;
}

void GAME_OVER(ESTADOS_MAQUINA estado_proximo_da_maquina)
{
	lcd_clear();

	if(estado_proximo_da_maquina == TURNO_P1){
		lcd_put_cur(0,0);
		lcd_send_string(game_over);
		lcd_put_cur(1,0);
		lcd_send_string(player_2_win);
	}

	else if(estado_proximo_da_maquina == TURNO_P2){
		lcd_put_cur(0,0);
		lcd_send_string(game_over);
		lcd_put_cur(1,0);
		lcd_send_string(player_1_win);
		}
}


void reset_game(){
	tens_minutes_counter_player_1 = 1;
	tens_seconds_counter_player_1 = 0;
	units_minutes_counter_player_1 = 0;
	units_seconds_counter_player_1 = 0;
															//Variáveis de posição para andar no "vetor de números."
	tens_minutes_counter_player_2 = 1;
	tens_seconds_counter_player_2 = 0;
	units_minutes_counter_player_2 = 0;
	units_seconds_counter_player_2 = 0;

	estado_proximo_da_maquina = PAUSE;
	HAL_Delay(5000);
}
