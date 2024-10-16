/*
 * app.h
 *
 *  Created on: Oct 14, 2024
 *      Author: eufil
 */

#ifndef APP_H_
#define APP_H_

#define FREQUECIA_DE_DERROTA
#define FREQUENCIA_DE_PAUSE

void app_init(void);

void app_loop(void);

void toca_buzzer(float frequencia_de_toque);

#endif /* APP_H_ */
