/*
 * APP_config.h
 *
 * Created: 5/11/2024 9:22:17 PM
 *  Author: VTE
 */ 


#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#define F_CPU 8000000UL

// Define dummy data for SPI
#define DUMMY_DATA 0xff

// Port and Pin Definitions
#define LDR_LED1_PORT 'A'
#define LDR_LED2_PORT 'C'
#define LDR_LED1_PIN1 6
#define LDR_LED2_PIN2 4

#define BUZZER_ALARM_PORT 'C'
#define BUZZER_ALARM_PIN 2

#define BUZZER_SOMKE_PORT 'C'
#define BUZZER_SMOKE_PIN 5

#define BUTTON_PORT 'C'
#define BUTTON_PIN 3
/********/
#define SetClock 'j'
#define SetAlarm 'k'

// ADC Channel for temperature reading
#define SMOKE_CHANNEL 7
#define LDR_CHANNEL 0

// Seven Segment Display Port and Pin Definitions
#define SEVEN_SEG_PORT 'D'
#define SEVEN_SEG_ENABLE_PORT1 'B'
#define SEVEN_SEG_ENABLE_PIN0 0
#define SEVEN_SEG_ENABLE_PIN1 1
#define SEVEN_SEG_ENABLE_PIN2 2
#define SEVEN_SEG_ENABLE_PIN3 3
#define SEVEN_SEG_ENABLE_PORT2 'A'
#define SEVEN_SEG_ENABLE_PIN4 4
#define SEVEN_SEG_ENABLE_PIN5 5

// smoke sensor
#define Factor 7.2
#define default_Value_Smoke 50
// EEPROM States
#define ON '1'
#define OFF '2'
/*****************/
#endif /* APP_CONFIG_H_ */