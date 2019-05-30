#include "ADC_config.h"
#include "SPI_config.h"

int main(void) {
	uint16_t TemperatureBuffer[NUMBER_OF_SPI_ADC_CHANNELS]; 
	uint16_t CurrentAndVoltageBuffer[NUMBER_OF_ADC_CHANNELS];
	uint8_t temperature, current, voltage, current_and_voltage;
	uint8_t CAN_Interrupt_Flag; // will eventually be a timer 
	PeripheralInit_SPI1();
	PeripheralInit_ADC1();
	//CANInit();
	while (1) {
		temperature = get_and_check_temperature_readings(TemperatureBuffer);
		current_and_voltage = get_and_check_current_and_voltage_readings(CurrentAndVoltageBuffer);
		current = current_and_voltage & CURRENT_MASK;
		voltage = current_and_voltage & VOLTAGE_MASK;
		if (current || voltage || temperature || CAN_Interrupt_Flag) {
			// send data over CAN
			
		}
	}
}
