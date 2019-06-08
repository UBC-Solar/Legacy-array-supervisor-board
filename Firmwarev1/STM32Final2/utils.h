#include "stm32f10x.h"
#include "ADC_config.h"
#include "SPI_config.h"
#include "CAN.h"

#define CAN_MESSAGE_LENGTH 8
#define LENGTH_OF_READING 2
#define CRITICAL_ERROR_ID 0x500
#define CRITICAL_OCD_ERROR_ID 0x501
#define NON_CRITICAL_ERROR_ID 0x502
#define TEMPERATURE_MSG_0 0x510
#define TEMPERATURE_MSG_1 0x511
#define TEMPERATURE_MSG_2 0x512
#define CURRENT_MSG_0 0x520
#define CURRENT_MSG_1 0x521
#define VOLTAGE_MSG_0 0x530

/*
Initializes GPIO peripherals for LEDs, contactors, etc. 
*/
void PeripheralInit_GPIO(void);

/*
Disconnects contactors
*/
void Disconnect_Contactors(void);

/*
Writes to LEDs with appropriate warning/error codes.
Green: Power on
Yellow: temperature error
Red: current/voltage error
*/
void Set_LEDS(uint16_t temperature_error, uint16_t current_error, uint16_t voltage_error);

/*
Sends CAN readings over CAN
Parameters: Pointers to temperatures, currents, and voltages to be sent
temperatures has length NUMBER_OF_SPI_ADC_CHANNELS
currents has length NUMBER_OF_CURRENT_CHANNELS
voltages has length NUMBER_OF_VOLTAGE_CHANNELS
*/
void Send_CAN_Data(uint16_t * temperatures, uint16_t * currents, uint16_t * voltages);

/*
Checks OCD pins.
Returns 1 if OCD. 0 otherwise. 
*/
uint8_t Check_OCD(void);
