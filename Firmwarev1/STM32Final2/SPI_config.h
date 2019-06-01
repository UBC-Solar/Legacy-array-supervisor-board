#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define SPI_ADC_MASK  0x0C00
#define SPI_ADC_OFFSET 12
#define NUMBER_OF_SPI_ADC_CHANNELS 11

/*
Measures the values of all SPI ADC channels and writes them to buffer
*/
void Get_Temperature_Readings(uint16_t * buffer);

/*
Transfers 16-bit output to device connected on SPI1
output is 16-bits and a valid SPI command for the 
connected device
Returns the 16-bit response from the SPI device
*/
uint16_t Transfer_16b_SPI1_Master(uint16_t output);

/*
Initializes SPI1 Peripheral
*/
void PeripheralInit_SPI1(void);


/*
Delays for counts number of microseconds
*/
void MicrosecondDelay(uint32_t counts);

