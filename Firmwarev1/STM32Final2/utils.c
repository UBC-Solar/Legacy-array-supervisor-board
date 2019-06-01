#include "utils.h"

void PeripheralInit_GPIO(void) {
	// initializes contactor and LED pins
}

void Disconnect_Contactors(void) {
	
}

void Set_LEDS(uint16_t temperature_error, uint16_t current_error, uint16_t voltage_error) {
	
}

void Send_CAN_Data(uint16_t * temperatures, uint16_t * currents, uint16_t * voltages) {
	uint8_t i, j, k;
	for (i = 0; i < NUMBER_OF_SPI_ADC_CHANNELS; i++) {
		
	}
	for (j = 0; j < NUMBER_OF_CURRENT_ADC_CHANNELS; j++) {
		
	}
	for (k = 0; k < NUMBER_OF_VOLTAGE_ADC_CHANNELS; k++) {
		
	}
}

uint8_t Check_OCD(void) {
}
