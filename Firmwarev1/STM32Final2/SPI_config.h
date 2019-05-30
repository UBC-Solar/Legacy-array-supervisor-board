#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define SPI_ADC_MASK  0x0C00
#define SPI_ADC_OFFSET 12
#define NUMBER_OF_SPI_ADC_CHANNELS 11
#define TEMPERATURE_THRESHOLD 0xFF00

/*
Measures values of temperature sensors, ensures that they're
within guidelines, and writes them to the buffer.
Does a full scan of all channels on the ADC. Returns 1 if 
any of them exceed TEMPERATURE_THRESHOLD
*/
uint8_t get_and_check_temperature_readings(uint16_t * buffer);

/*
Does a full scan of all connected temperature sensors on ADC over SPI
and writes it to buffer
buffer is at least SPI_ADC_CHANNEL_NUMBER long
*/
void temperature_readings_SPI1(uint16_t * buffer); 

/*
Transfers 16-bit output to device connected on SPI1
output is 16-bits and a valid SPI command for the 
connected device
Returns the 16-bit response from the SPI device
*/
uint16_t transfer_16b_SPI1_Master(uint16_t output);

/*
Initializes SPI1 Peripheral
buffer points to buffer to store values read
*/
void PeripheralInit_SPI1(void);


/*
Delays for counts number of microseconds
*/
void MicrosecondDelay(uint32_t counts);

/*
Converts a voltage reading to the appropriate temperature
*/
//todo
double Get_Temperature(uint16_t reading);
