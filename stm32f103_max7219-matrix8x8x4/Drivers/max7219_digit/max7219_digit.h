#pragma once

// correct in "max7219_digit.c" PORT and PIN for CS-strob

void max7219_print_number(SPI_HandleTypeDef * hspi, int number,int position);
void max7219_print_one_digit(SPI_HandleTypeDef * hspi,int position,int digit);
void max7219_init(SPI_HandleTypeDef * hspi);
void max7219_write_strob(void);

//  example for SPI1:
//
//  max7219_init(&hspi1);
// 	max7219_print_number(&hspi1,0000,0);
//	max7219_print_number(&hspi1,1111,1);



