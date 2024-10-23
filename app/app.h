/*
 * app.h
 *
 *  Created on: Oct 14, 2024
 *      Author: eufil
 */

#ifndef APP_H_
#define APP_H_

#define TEMPO_TOTAL 1800000
#define FREQUECIA_DE_DERROTA 350
#define FREQUENCIA_DE_PAUSE 669



typedef enum
{
  PAUSE = 0,
  TURNO_P1,
  TURNO_P2
}ESTADOS_MAQUINA;

typedef enum
{
  CORRENDO = 0,
  DERROTA_P1,
  DERROTA_P2
}ESTADOS_JOGO;

typedef struct
{
	GPIO_PinState ESTADO_PINO_TROCA_PARA_P2;
	GPIO_PinState ESTADO_PINO_TROCA_PARA_P1;
}ESTADOS_DOS_PINOS;

void app_init(void);

void app_loop(void);

void toca_buzzer(float frequencia_de_toque);

void para_buzzer();

ESTADOS_MAQUINA troca_de_estado(ESTADOS_DOS_PINOS estado_atual);

ESTADOS_DOS_PINOS checa_estados_pinos();

#endif /* APP_H_ */
