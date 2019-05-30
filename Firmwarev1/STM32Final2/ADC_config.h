#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define CURRENT_MASK 1
#define CURRENT 1
#define VOLTAGE_MASK 2
#define VOLTAGE 2
#define NUMBER_OF_ADC_CHANNELS 9
#define NUMBER_OF_VOLTAGE_ADC_CHANNELS 3
#define NUMBER_OF_CURRENT_ADC_CHANNELS 6
#define CURRENT_THRESHOLD 0xABCD
#define VOLTAGE_THRESHOLD 0xABCD

/*
Measures values of current and voltage sensors, ensures that they're
within guidelines, and writes them to the buffer.
Does a full scan of all channels on the ADC. Returns 1 if 
any of them exceed CURRENT or VOLTAGE threshold (depending on the channel)
*/
uint8_t get_and_check_current_and_voltage_readings(uint16_t * buffer);

/*
Does a full scan of all connected sensors on defined ADC channels over SPI
and writes it to buffer
buffer is at least SPI_ADC_CHANNEL_NUMBER long
*/
void get_adc_readings(uint16_t * buffer);

/*
Gets the value converted from a single ADC
*/
uint16_t Get_ADC_Converted_Value(uint8_t channel);

/*
Configures ADC channels for use
*/
void Configure_ADC_Channels(void);


/*
Initializes ADC peripherals
*/
void PeripheralInit_ADC1(void);

/*
Converts a voltage reading to its real voltage
*/
//todo
double Get_Voltage(uint16_t reading); 

/*
Converts a voltage reading to the appropriate current
*/
//todo
double Get_Current(uint16_t reading);

//void PeripheralInit_DMA1(uint16_t * ADCBuf);
/*
void PeripheralInit_DMA1(uint16_t * ADCBuf) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef  DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	//disable memory to memory: doing ADC to MEM
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//setting normal mode (non circular)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//high priority
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//source and destination data size word=16bit
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//automatic memory destination increment enable.
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//source address increment disable
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//Location assigned to peripheral register will be destination
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//chunk of data to be transfered
	DMA_InitStructure.DMA_BufferSize = NUMBER_OF_ADC_CHANNELS * 4;
	//source and destination start addresses
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC_DR_DATA;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADCBuf;
	//send values to DMA registers
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// Enable DMA1 Channel Transfer Complete interrupt
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
	NVIC_InitTypeDef NVIC_InitStructure;
	//Enable DMA1 channel IRQ Channel 
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
*/
