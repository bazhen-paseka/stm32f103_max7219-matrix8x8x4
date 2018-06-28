#include "stm32f1xx_hal.h"
#include "max7219_digit.h"

#define CS_PORT GPIOB
#define CS_PIN  GPIO_PIN_6

void max7219_write_strob(void)
{
	HAL_Delay(1);
	HAL_GPIO_WritePin(CS_PORT,CS_PIN,SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(CS_PORT,CS_PIN,RESET);
	HAL_Delay(1);
}

void max7219_print_number(SPI_HandleTypeDef *hspi,int number,int position)
{
	if (position == 0) position = 4;
	else 			position = 0;
	max7219_print_one_digit(hspi, 4 + position, (number/1000) % 10 );
	max7219_print_one_digit(hspi, 3 + position, (number/100 ) % 10 );
	max7219_print_one_digit(hspi, 2 + position, (number/10  ) % 10 );
	max7219_print_one_digit(hspi, 1 + position, (number     ) % 10 );
}

void max7219_print_one_digit(SPI_HandleTypeDef *hspi,int position,int digit)
{
	uint8_t myTrans[2];
	myTrans[0] = position	; // razryad
	myTrans[1] = digit	; // znachenie
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();
}

void max7219_init(SPI_HandleTypeDef *hspi)
{
	uint8_t myTrans[2];
	HAL_GPIO_WritePin(CS_PORT,CS_PIN,RESET);

	// test - On
	myTrans[0] = 0x0F;  myTrans[1] = 0x01;
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();

	HAL_Delay(1000);

	// test - Off
	myTrans[0] = 0x0F;  myTrans[1] = 0x00;
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();

	// Decode Mode - No 1 in 1
	// myTrans[1] hex   -> FF
	// myTrans[1] pixel -> 00
	int DecodeMode = 0xFF;
	myTrans[0] = 0x09;  myTrans[1] = DecodeMode;	// pixel
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();

	// Intensity 3/32 0x01,
	myTrans[0] = 0x0A;  myTrans[1] = 0x03;
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();

	//Scan Limit - All
	myTrans[0] = 0x0B;  myTrans[1] = 0x07;
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();

	// Shutdown - none
	// myTrans[1] -> 00 sleep
	// myTrans[1] -> 01 work
	myTrans[0] = 0x0C;  myTrans[1] = 0x01;
	HAL_SPI_Transmit(hspi,myTrans,2,1);
	max7219_write_strob();
}
