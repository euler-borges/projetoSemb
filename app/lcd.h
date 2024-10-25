/*
 * lcd.h
 *
 *  Created on: Oct 23, 2024
 *      Author: Luiz Felipe Spinola Silva
 */


#ifndef LCD_H_
#define LCD_H_


void lcd_init (void);   													// initialize lcd

void lcd_send_cmd (char cmd);  						    					// send command to the lcd

void lcd_send_data (char data);                         					// send data to the lcd

void lcd_send_string (char *str);  											// send string to the lcd

void lcd_put_cur(int row, int col);  										// put cursor at the entered position row (0 or 1), col (0-19);

void lcd_clear (void); 														// limpa o lcd													//acaba inicialização

void decrease_one_second (ESTADOS_MAQUINA estado_proximo_da_maquina); 		//diminui um segundo no relógio



#endif
