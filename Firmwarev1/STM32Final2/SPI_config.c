#include "SPI_config.h"

const uint16_t SPI_ADC_CHANNELS[NUMBER_OF_SPI_ADC_CHANNELS] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA};

/*
Measures the values of all SPI ADC channels and writes them to buffer
*/
void Get_Temperature_Readings(uint16_t * buffer) {
	MicrosecondDelay(10);
	Transfer_16b_SPI1_Master(SPI_ADC_CHANNELS[0] << SPI_ADC_OFFSET | SPI_ADC_MASK); // transfer first byte to start conversion
	for (uint8_t i = 1; i <= NUMBER_OF_SPI_ADC_CHANNELS; i++) {
		MicrosecondDelay(10);
		buffer[i - 1] = Transfer_16b_SPI1_Master(SPI_ADC_CHANNELS[i] << SPI_ADC_OFFSET | SPI_ADC_MASK); //read current value while transferring next value due to how ADC behaves
	}
}

/*
Transfers 16-bit output to device connected on SPI1
output is 16-bits and a valid SPI command for the 
connected device
Returns the 16-bit response from the SPI device
*/
uint16_t Transfer_16b_SPI1_Master(uint16_t output) {
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, output);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	uint16_t ret = SPI_I2S_ReceiveData(SPI1);
	return ret;
}

/*
Initializes SPI1 Peripheral
*/
void PeripheralInit_SPI1(void) {
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
	
	// Two lines: MOSI and MISO
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	// Configure as master
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	// 16 bit transfer size
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	//MSB first
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	//Configure transmission method
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	// Use software chip select
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB2 72/64 = 1.125 MHz
  // SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // APB2 72/256 = 0.28 MHz
  // SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // APB2 72/16 = 4.5 MHz
	SPI_InitStruct.SPI_CRCPolynomial = 7; // reset
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*
Delays for counts number of microseconds
*/
void MicrosecondDelay(uint32_t counts)
{
	uint32_t i;
	uint8_t j;

	for (i = 0; i < counts; i++)
	{
		for (j = 0; j<16; j++){}
	}
}

