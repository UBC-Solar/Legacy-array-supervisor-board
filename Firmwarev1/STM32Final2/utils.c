#include "utils.h"

/*
Initializes GPIO peripherals for LEDs, contactors, etc. 
*/
void PeripheralInit_GPIO(void) {
	// initializes contactor and LED pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	// Relay control
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// LED control
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// ADC_EOC + I_OUT_3_OCD
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// I_IN_3_OCD + I_OUT_1_OCD + I_IN_1_OCD + I_OUT_2_OCD
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// I_IN_2_OCD
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/*
Set contactors

*/
void Set_Contactors(uint8_t value) {
	if (value == 1) {
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	} else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);

	}
}

/*
Writes to LEDs with appropriate warning/error codes.
Green: Power on
Yellow: temperature error
Red: current/voltage error
*/
void Set_LEDS(uint16_t temperature_error, uint16_t current_error, uint16_t voltage_error) {
	GPIO_SetBits(GPIOC, GPIO_Pin_9); // power
	if (current_error | voltage_error) { // critical error
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
	} else {
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	}
	if (temperature_error) { // non critical error
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
	} else {
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	}
}

/*
Sends CAN readings over CAN
Parameters: Pointers to temperatures, currents, and voltages to be sent
temperatures has length NUMBER_OF_SPI_ADC_CHANNELS
currents has length NUMBER_OF_CURRENT_CHANNELS
voltages has length NUMBER_OF_VOLTAGE_CHANNELS
*/
void Send_CAN_Data(uint16_t * temperatures, uint16_t * currents, uint16_t * voltages) {
	uint8_t i, j, k;
	uint8_t arr[CAN_MESSAGE_LENGTH];
	CAN_msg_t tx;
	
	uint8_t readings_per_message = CAN_MESSAGE_LENGTH / LENGTH_OF_READING;
	
	for (i = 0; i < NUMBER_OF_SPI_ADC_CHANNELS; i++) { // parses whole array
		arr[i % CAN_MESSAGE_LENGTH] = temperatures[i] & 0xFF;
		arr[(i + 1) % CAN_MESSAGE_LENGTH] = temperatures[i] >> 8;
		if (i % readings_per_message == readings_per_message - 1) { // if message is full
			tx = (CAN_msg_t) {TEMPERATURE_MSG_0 + i / readings_per_message, // message IDs are offset from MSG_0
				{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]}, (i % readings_per_message + 1) * LENGTH_OF_READING}; 
			CANSend(&tx);
		}
	}
	if (i % readings_per_message != 0 && i == NUMBER_OF_SPI_ADC_CHANNELS) { // sends final message if necessary
		tx = (CAN_msg_t) {TEMPERATURE_MSG_0 + i / readings_per_message, 
				{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]}, (i % readings_per_message + 1) * LENGTH_OF_READING};
		CANSend(&tx);
	}
	
	for (j = 0; j < NUMBER_OF_CURRENT_ADC_CHANNELS; j++) {
		arr[j % CAN_MESSAGE_LENGTH] = currents[j] & 0xFF;
		arr[(j + 1) % CAN_MESSAGE_LENGTH] = currents[j] >> 8;
		if (j % readings_per_message == readings_per_message - 1) {
			tx = (CAN_msg_t) {CURRENT_MSG_0 + j / readings_per_message, 
					{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]}, (j % readings_per_message + 1) * LENGTH_OF_READING};
			CANSend(&tx);
		}
	}
	if (j % readings_per_message != 0 && j == NUMBER_OF_CURRENT_ADC_CHANNELS) {
		tx = (CAN_msg_t) {CURRENT_MSG_0 + j / readings_per_message, 
				{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]}, (j % readings_per_message + 1) * LENGTH_OF_READING};
		CANSend(&tx);
	}
	
	for (k = 0; k < NUMBER_OF_VOLTAGE_ADC_CHANNELS; k++) {
		arr[k % CAN_MESSAGE_LENGTH] = voltages[k] & 0xFF;
		arr[(k + 1) % CAN_MESSAGE_LENGTH] = voltages[k] >> 8;
		if (k % readings_per_message == readings_per_message - 1) {
			tx = (CAN_msg_t) {VOLTAGE_MSG_0 + k / readings_per_message, 
					{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]}, (k % readings_per_message + 1) * LENGTH_OF_READING};
			CANSend(&tx);
		}
	}
	if (k % readings_per_message != 0 && k == NUMBER_OF_VOLTAGE_ADC_CHANNELS) {
		tx = (CAN_msg_t) {VOLTAGE_MSG_0 + k / readings_per_message, 
				{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]}, (i % readings_per_message + 1) * LENGTH_OF_READING};
		CANSend(&tx);
	}
}

/*
Checks OCD pins.
Returns 1 if OCD. 0 otherwise. 
*/
uint8_t Check_OCD(void) {
	uint8_t ocd = 0;
	ocd |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	ocd |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
	ocd |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
	ocd |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
	ocd |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
	ocd |= GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	return ocd;
}
