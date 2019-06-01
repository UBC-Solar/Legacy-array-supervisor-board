#include "stm32f10x.h"
#include "ADC_config.h"
#include "SPI_config.h"
#include "CAN.h"

#define CRITICAL_ERROR_ID 0x500
#define CRITICAL_OCD_ERROR_ID 0x501
#define NON_CRITICAL_ERROR_ID 0x502
#define TEMPERATURE_MSG_0 0x510
#define TEMPERATURE_MSG_1 0x511
#define TEMPERATURE_MSG_2 0x512
#define CURRENT_MSG_0 0x520
#define CURRENT_MSG_1 0x521
#define VOLTAGE_MSG_0 0x530

void PeripheralInit_GPIO(void);
void Disconnect_Contactors(void);
void Set_LEDS(uint16_t temperature_error, uint16_t current_error, uint16_t voltage_error);
void Send_CAN_Data(uint16_t * temperatures, uint16_t * currents, uint16_t * voltages);
uint8_t Check_OCD(void);
