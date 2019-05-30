#include "ADC_config.h"

const uint16_t ADC_Channels[NUMBER_OF_ADC_CHANNELS] = {ADC_Channel_1, ADC_Channel_8, ADC_Channel_9, ADC_Channel_10, ADC_Channel_11, ADC_Channel_12, ADC_Channel_13, ADC_Channel_14, ADC_Channel_15};
const uint16_t ADC_Mapping[NUMBER_OF_ADC_CHANNELS] = {CURRENT, CURRENT, VOLTAGE, CURRENT, CURRENT, CURRENT, CURRENT, VOLTAGE, VOLTAGE};
const uint16_t Voltage_Channels[NUMBER_OF_VOLTAGE_ADC_CHANNELS] = {ADC_Channel_15, ADC_Channel_9, ADC_Channel_14};
const uint16_t Current_Channels[NUMBER_OF_CURRENT_ADC_CHANNELS] = {ADC_Channel_1, ADC_Channel_8, ADC_Channel_11, ADC_Channel_13, ADC_Channel_12, ADC_Channel_10};

uint8_t get_and_check_current_and_voltage_readings(uint16_t * buffer) {
	for (uint8_t i = 0; i < NUMBER_OF_ADC_CHANNELS; i++) {
		buffer[i] = Get_ADC_Converted_Value(ADC_Channels[i]);
		if (ADC_Mapping[i] == CURRENT) {
			if (buffer[i] > CURRENT_THRESHOLD) {
				//do something 
				return CURRENT;
			}
		} else {
			if (buffer[i] > VOLTAGE_THRESHOLD) {
				//do something
				return VOLTAGE;
			}
		}
	}
	return 0;
}

void get_adc_readings(uint16_t * buffer) {
	for (uint8_t i = 0; i < NUMBER_OF_ADC_CHANNELS; i++) {
		buffer[i] = Get_ADC_Converted_Value(ADC_Channels[i]);
	}
}

/*
Converts value from ADC channel
*/
uint16_t Get_ADC_Converted_Value(uint8_t channel) {
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_7Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}

void Configure_ADC_Channels(void) {
	// Configure all channels with sample time, channel, rank
	for (uint8_t i = 0; i < NUMBER_OF_ADC_CHANNELS; i++) {
		ADC_RegularChannelConfig(ADC1, ADC_Channels[i], i + 1, ADC_SampleTime_7Cycles5);
	}
}

void PeripheralInit_ADC1(void) {
	ADC_DeInit(ADC1); //deinitialize to reconfigure
	ADC_InitTypeDef ADC_InitStruct;
	// Enable RCC clk
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	// Right align data
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	// keep scanning 
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
	// continuous
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStruct.ADC_NbrOfChannel = NUMBER_OF_ADC_CHANNELS;
	// no external trigger
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1, &ADC_InitStruct);
	Configure_ADC_Channels();
	ADC_Cmd(ADC1, ENABLE);
	//ADC_DMACmd(ADC1, ENABLE);
	// Calibrate ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	//Set up GPIO pins as AIN
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

