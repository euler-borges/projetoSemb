/*
 * lcd.c
 *
 *  Created on: Oct 23, 2024
 *      Author: Luiz Felipe Spinola Silva
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "app.h"
#include "hw.h"
#include "lcd.h"

volatile uint8_t tens_minutes_counter_player_1 = 1;
volatile uint8_t tens_seconds_counter_player_1 = 0;
volatile uint8_t units_minutes_counter_player_1 = 0;
volatile uint8_t units_seconds_counter_player_1 = 0;
														//Variáveis de posição para andar no "vetor de números."
volatile uint8_t tens_minutes_counter_player_2 = 1;
volatile uint8_t tens_seconds_counter_player_2 = 0;
volatile uint8_t units_minutes_counter_player_2 = 0;
volatile uint8_t units_seconds_counter_player_2 = 0;

char* numbers =  "0123456789";

char* player_1 = "PLAYER 1";							//Nome dos players
char* player_2 = "PLAYER 2";

char* player_1_win = "PLAYER 1 WINS";
char* player_2_win = "PLAYER 2 WINS";
char* game_over = "GAME OVER";

extern I2C_HandleTypeDef hi2c1;  						// change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x78 							// endereço i2c escravo NHD-C0220BiZ-FSW-FBW-3V3M

#define PLAYER1_UNITS_SECONDS_POS 3
#define PLAYER1_TENS_SECONDS_POS 2
#define PLAYER1_UNITS_MINUTES_POS 1
#define PLAYER1_TENS_MINUTES_POS 0
														//Valores equivalentes a posição da coluna onde cada unidade vai ser
#define PLAYER2_UNITS_SECONDS_POS 9						//imprimida.
#define PLAYER2_TENS_SECONDS_POS 8
#define PLAYER2_UNITS_MINUTES_POS 7
#define PLAYER2_TENS_MINUTES_POS 6

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  							//en=1, rs=0 -> bxxxx1100
	data_t[1] = data_u|0x08;  							//en=0, rs=0 -> bxxxx1000
	data_t[2] = data_l|0x0C;  							//en=1, rs=0 -> bxxxx1100
	data_t[3] = data_l|0x08;  							//en=0, rs=0 -> bxxxx1000
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0 -> bxxxx1101
	data_t[1] = data_u|0x09;  //en=0, rs=0 -> bxxxx1001
	data_t[2] = data_l|0x0D;  //en=1, rs=0 -> bxxxx1101
	data_t[3] = data_l|0x09;  //en=0, rs=0 -> bxxxx1001
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)
{
	lcd_send_cmd (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);					//Inicialização do modo de transmissão de 4 bits
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd(SLAVE_ADDRESS_LCD);//Slave=0x78
	lcd_send_cmd(0x00);//Comsend = 0x00
	lcd_send_cmd(0x38);
	HAL_Delay(10);
	lcd_send_cmd(0x39);
	HAL_Delay(10);					//Inicialização Padrão do Datasheet
	lcd_send_cmd(0x14);
	lcd_send_cmd(0x78);
	lcd_send_cmd(0x5E);
	lcd_send_cmd(0x6D);
	lcd_send_cmd(0x0C);
	lcd_send_cmd(0x01);
	lcd_send_cmd(0x06);
	HAL_Delay(10);

	// Printar Valores Iniciais na tela

	lcd_put_cur(0,0);
	lcd_send_string(player_1);				//printando nomes
	lcd_put_cur(0,9);
	lcd_send_string(player_2);

	lcd_put_cur(1, PLAYER1_UNITS_SECONDS_POS);
	lcd_send_data(numbers[units_seconds_counter_player_1]);

	lcd_put_cur(1, PLAYER1_TENS_SECONDS_POS);
	lcd_send_data(numbers[tens_seconds_counter_player_1]);
																	//Printando Relógio Player 1
	lcd_put_cur(1, PLAYER1_UNITS_MINUTES_POS);
	lcd_send_data(numbers[units_minutes_counter_player_1]);

	lcd_put_cur(1, PLAYER1_TENS_MINUTES_POS);
	lcd_send_data(numbers[tens_minutes_counter_player_1]);



	lcd_put_cur(1, PLAYER2_UNITS_SECONDS_POS);
	lcd_send_data(numbers[units_seconds_counter_player_2]);

	lcd_put_cur(1, PLAYER2_TENS_SECONDS_POS);
	lcd_send_data(numbers[tens_seconds_counter_player_2]);
																	//Printando Relógio Player 2
	lcd_put_cur(1, PLAYER2_UNITS_MINUTES_POS);
	lcd_send_data(numbers[units_minutes_counter_player_2]);

	lcd_put_cur(1, PLAYER2_TENS_MINUTES_POS);
	lcd_send_data(numbers[tens_minutes_counter_player_2]);



}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void decrease_one_second(ESTADOS_MAQUINA estado_proximo_da_maquina)
{

	//Função para diminuir um segundo no relógio amostrado no display, foi utilzado uma lógica com ifs em cascata
	//para desencadear a mudança de outras unidades do relógio casoo valor em segundos já esteja em zero.

	if(estado_proximo_da_maquina == TURNO_P1)
	{
		if(units_seconds_counter_player_1 == 0){
		   units_seconds_counter_player_1 = 9;
		   lcd_put_cur(1, PLAYER1_UNITS_SECONDS_POS);
		   lcd_send_data(numbers[units_seconds_counter_player_1]);

		   if(tens_seconds_counter_player_1 == 0){
			  tens_seconds_counter_player_1 = 5;
			  lcd_put_cur(1, PLAYER1_TENS_SECONDS_POS);
			  lcd_send_data(numbers[tens_seconds_counter_player_1]);

			  if(units_minutes_counter_player_1 == 0){
			     units_minutes_counter_player_1 = 9;		//ERROOOOOO
			     lcd_put_cur(1, PLAYER1_UNITS_MINUTES_POS);
			     lcd_send_data(numbers[units_minutes_counter_player_1]);

			     if(tens_minutes_counter_player_1 == 0){
				    tens_minutes_counter_player_1 = 0;
				    lcd_put_cur(1, PLAYER1_TENS_MINUTES_POS);
				    lcd_send_data(numbers[tens_minutes_counter_player_1]);
			        }else{
				          tens_minutes_counter_player_1--;
				          lcd_put_cur(1, PLAYER1_TENS_MINUTES_POS);
				          lcd_send_data(numbers[tens_minutes_counter_player_1]);
			        }
			     }else{
				       units_minutes_counter_player_1--;
				       lcd_put_cur(1, PLAYER1_UNITS_MINUTES_POS);
				       lcd_send_data(numbers[units_minutes_counter_player_1]);
			     }
		      }else{
		    	    tens_seconds_counter_player_1--;
		 	        lcd_put_cur(1, PLAYER1_TENS_SECONDS_POS);
		 	        lcd_send_data(numbers[tens_seconds_counter_player_1]);
		      }
		   }else{
			     units_seconds_counter_player_1--;
			     lcd_put_cur(1, PLAYER1_UNITS_SECONDS_POS);
			     lcd_send_data(numbers[units_seconds_counter_player_1]);
		}
	}

	if(estado_proximo_da_maquina == TURNO_P2)
		{
			if(units_seconds_counter_player_2 == 0){
			   units_seconds_counter_player_2 = 9;
			   lcd_put_cur(1, PLAYER2_UNITS_SECONDS_POS);
			   lcd_send_data(numbers[units_seconds_counter_player_2]);

			   if(tens_seconds_counter_player_2 == 0){
				  tens_seconds_counter_player_2 = 5;
				  lcd_put_cur(1, PLAYER2_TENS_SECONDS_POS);
				  lcd_send_data(numbers[tens_seconds_counter_player_2]);

				  if(units_minutes_counter_player_2 == 0){
				     units_minutes_counter_player_2 = 9;
				     lcd_put_cur(1, PLAYER2_UNITS_MINUTES_POS);
				     lcd_send_data(numbers[units_minutes_counter_player_2]);

				     if(tens_minutes_counter_player_2 == 0){
					    tens_minutes_counter_player_2 = 0;
					    lcd_put_cur(1, PLAYER2_TENS_MINUTES_POS);
					    lcd_send_data(numbers[tens_minutes_counter_player_2]);
				        }else{
					          tens_minutes_counter_player_2--;
					          lcd_put_cur(1, PLAYER2_TENS_MINUTES_POS);
					          lcd_send_data(numbers[tens_minutes_counter_player_2]);
				        }
				     }else{
					       units_minutes_counter_player_2--;
					       lcd_put_cur(1, PLAYER2_UNITS_MINUTES_POS);
					       lcd_send_data(numbers[units_minutes_counter_player_2]);
				     }
			      }else{
			    	    tens_seconds_counter_player_2--;
			 	        lcd_put_cur(1, PLAYER2_TENS_SECONDS_POS);
			 	        lcd_send_data(numbers[tens_seconds_counter_player_2]);
			      }
			   }else{
				     units_seconds_counter_player_2--;
				     lcd_put_cur(1, PLAYER2_UNITS_SECONDS_POS);
				     lcd_send_data(numbers[units_seconds_counter_player_2]);
			}
		}



}

void pause_lcd(void){				//Muda estado do display para pausado.

	lcd_clear();
	lcd_put_cur(0,0);
	lcd_send_string("PAUSED");

}

void reload_lcd(void){
	//limpa tela e reescreve o relogio (como fez no init)
	lcd_clear();

	lcd_put_cur(0,0);
	lcd_send_string(player_1);				//printando nomes
	lcd_put_cur(0,9);
	lcd_send_string(player_2);

	lcd_put_cur(1, PLAYER1_UNITS_SECONDS_POS);
	lcd_send_data(numbers[units_seconds_counter_player_1]);

	lcd_put_cur(1, PLAYER1_TENS_SECONDS_POS);
	lcd_send_data(numbers[tens_seconds_counter_player_1]);
																		//Printando Relógio Player 1
	lcd_put_cur(1, PLAYER1_UNITS_MINUTES_POS);
	lcd_send_data(numbers[units_minutes_counter_player_1]);

	lcd_put_cur(1, PLAYER1_TENS_MINUTES_POS);
	lcd_send_data(numbers[tens_minutes_counter_player_1]);



	lcd_put_cur(1, PLAYER2_UNITS_SECONDS_POS);
	lcd_send_data(numbers[units_seconds_counter_player_2]);

	lcd_put_cur(1, PLAYER2_TENS_SECONDS_POS);
	lcd_send_data(numbers[tens_seconds_counter_player_2]);
																		//Printando Relógio Player 2
	lcd_put_cur(1, PLAYER2_UNITS_MINUTES_POS);
	lcd_send_data(numbers[units_minutes_counter_player_2]);

	lcd_put_cur(1, PLAYER2_TENS_MINUTES_POS);
	lcd_send_data(numbers[tens_minutes_counter_player_2]);
}

