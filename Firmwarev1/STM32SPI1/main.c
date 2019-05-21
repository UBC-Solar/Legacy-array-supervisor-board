#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void delay(unsigned int nCount);
void PeripheralInit_SPI1_Master(void);
void PeripheralInit_SPI1_GPIO(void);
uint16_t transfer_16b_SPI1_Master(uint16_t output);

void PeripheralInit_SPI1_AF_Master(void);
void PeripheralInit_SPI1_AF_GPIO(void);
uint16_t transfer_16b_SPI1_AF_Master(uint16_t output);

int main (void)
{
  PeripheralInit_SPI1_Master();
	PeripheralInit_SPI1_GPIO();
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	uint16_t response = transfer_16b_SPI1_Master(0xDC00);
	return 0;
}

uint16_t transfer_16b_SPI1_Master(uint16_t output) {
	//while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, output);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

void PeripheralInit_SPI1_GPIO(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void PeripheralInit_SPI1_Master(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	GPIO_StructInit(&GPIO_InitStruct);
	SPI_StructInit(&SPI_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	
	//initialize A5/SCK
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//initialize A6/MISO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//initialize A7/MOSI
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB2 72/64 = 1.125 MHz
  // SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // APB2 72/256 = 0.28 MHz
  // SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // APB2 72/16 = 4.5 MHz
	SPI_InitStruct.SPI_CRCPolynomial = 7; // reset
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
	
	
}
 
// Delay function
void delay(unsigned int nCount)
{
  unsigned int i, j;
 
  for (i = 0; i < nCount; i++) {
    for (j = 0; j < 0x2AFF; j++) {
		}
	}
}
