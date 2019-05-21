#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void PeripheralInit_ADC_GPIO(void);
void PeripheralInit_ADC(void);
int Get_ADC_Converted_Value(uint8_t channel);
double Get_Voltage(uint8_t channel); // need to implement this: does stm32 not use IEEE floating point?

int main(void) {
	PeripheralInit_ADC_GPIO();
	PeripheralInit_ADC();
	int ADCValue = Get_ADC_Converted_Value(ADC_Channel_0);
	return 0;
}

int Get_ADC_Converted_Value(uint8_t channel) {
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_7Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}

void PeripheralInit_ADC(void) {
	ADC_DeInit(ADC1); //deinitialize to reconfigure
	ADC_InitTypeDef ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_NbrOfChannel = ADC_Channel_0;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

void PeripheralInit_ADC_GPIO(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStruct);
	//setup appropriate pins
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
