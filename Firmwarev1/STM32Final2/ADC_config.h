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


/*
Measures values of current and voltage sensors and writes them to
appropriate arrays
*/
void Get_Current_Voltage_Readings(uint16_t * currents, uint16_t * voltages);

/*
Gets the value converted from a single ADC channel
Parameters: Channel to read from
Returns: 12-bit value read from ADC 
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
