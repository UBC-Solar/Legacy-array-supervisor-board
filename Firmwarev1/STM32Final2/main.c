#include "ADC_config.h"
#include "SPI_config.h"
#include "utils.h"
#include "CAN.h"

#define TEMPERATURE_CONVERSION 0xABCD
#define CURRENT_CONVERSION 0xABCD
#define VOLTAGE_CONVERSION 0xABCD
#define TEMPERATURE_THRESHOLD 9000
#define CURRENT_THRESHOLD 9000
#define VOLTAGE_THRESHOLD 9000


CAN_msg_t CAN_rx_msg;
CAN_msg_t CAN_tx_msg;

int main(void) {
	uint16_t temperatures[NUMBER_OF_SPI_ADC_CHANNELS]; 
	uint16_t currents[NUMBER_OF_CURRENT_ADC_CHANNELS];
	uint16_t voltages[NUMBER_OF_VOLTAGE_ADC_CHANNELS];
	uint8_t i, j, k;
	uint16_t temperature_error, current_error, voltage_error;

	uint8_t CAN_Interrupt_Flag = 0; // will eventually be a timer 
	PeripheralInit_SPI1();
	PeripheralInit_ADC1();
	PeripheralInit_GPIO();
	CANInit();
	while (1) {
		// gets readings
		Get_Temperature_Readings(temperatures); 
		Get_Current_Voltage_Readings(currents, voltages);
		
		// processes readings and checks if they exceed thresholds
		for (i = 0; i < NUMBER_OF_SPI_ADC_CHANNELS; i++) {
			temperatures[i] = TEMPERATURE_CONVERSION * (temperatures[i] >> 4);
			if (temperatures[i] > TEMPERATURE_THRESHOLD) {
				temperature_error |= 1 << i;
			}
		}
		for (j = 0; j < NUMBER_OF_CURRENT_ADC_CHANNELS; j++) {
			currents[i] = CURRENT_CONVERSION * (currents[i] >> 4);
			if (currents[i] > CURRENT_THRESHOLD) {
				current_error |= 1 << i;
			}
		}
		for (j = 0; j < NUMBER_OF_VOLTAGE_ADC_CHANNELS; j++) {
			voltages[i] = VOLTAGE_CONVERSION * (voltages[i] >> 4);
			if (voltages[i] > VOLTAGE_THRESHOLD) {
				voltage_error |= 1 << i;
			}
		}
		
		uint8_t ocd = Check_OCD();
		if (ocd) { // if we hit OCD limits, warn the MCN and shutdown
			Disconnect_Contactors();
			CAN_msg_t current_error_message = {CRITICAL_OCD_ERROR_ID, {ocd}, 1};
			CANSend(&current_error_message);
			break;
		}
		Set_LEDS(temperature_error, current_error, voltage_error); 
		if (current_error || voltage_error) { // if we encounter a critical error that may damage components, warn the MCN and shutdown
			Disconnect_Contactors();
			uint8_t currlo = current_error >> 8;
			uint8_t currhi = current_error & 0xFF;
			uint8_t voltlo = voltage_error >> 8;
			uint8_t volthi = voltage_error & 0xFF;
			CAN_msg_t critical_error_message = {CRITICAL_ERROR_ID, {currlo, currhi, voltlo, volthi}, 4};
			CANSend(&critical_error_message);
			break;
		}
		if (temperature_error) { // if panel begins to overheat, warn the MCN
			uint8_t lo = temperature_error >> 8;
			uint8_t hi = temperature_error & 0xFF;
			CAN_msg_t noncritical_error_message = {NON_CRITICAL_ERROR_ID, {lo, hi}, 2};
			CANSend(&noncritical_error_message);
		}
		
		
		if (CAN_Interrupt_Flag) { // interrupt sets this flag, which triggers a data send
			CAN_Interrupt_Flag = 0;
			Send_CAN_Data(temperatures, currents, voltages);
		}
	}
}