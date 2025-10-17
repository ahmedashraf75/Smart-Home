/*
 * APP_config.h
 *
 * Created: 6/11/2024 9:22:17 PM
 *  Author: VTE
 */ 


#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#define F_CPU 8000000UL

// Define dummy data for SPI
#define DUMMY_DATA 0xff

// Define EEPROM locations
#define LED_LOCATION1  0x31
#define LED_LOCATION2  0x32
#define FAN_LOCATION1  0x33
#define FAN_LOCATION2  0x34
#define DOOR_LOCATION1 0x35
#define DOOR_LOCATION2 0x36
#define TV_LOCATION    0x37
#define AIR_LOCATION1  0x30
#define AIR_STATUS     0x38
#define STATUES_LOCATION 0x42
#define REQUIRED_TEMPERATURE_LOCATION1 0x39
#define REQUIRED_TEMPERATURE_LOCATION2 0x40

// Port and Pin Definitions
#define LED_PORT 'B'
#define LED_PIN1 0
#define LED_PIN2 3

#define FAN_PORT 'A'
#define FAN_PIN1 6
#define FAN_PIN2 7

#define AIR_CONDITIONER_PORT 'A'
#define AIR_CONDITIONER_PIN 2

#define TV_PORT 'D'
#define TV_PIN 3

#define DOOR_TIMER1_WAVE_close 1.49999
#define DOOR_TIMER1_WAVE_open 0.99999

// ADC Channel for temperature reading
#define ADC_CHANNEL 0

// L293D Port and Pin Definitions
#define L293D_PORT 'D'
#define L293D_IN2 0
#define L293D_EN 1
#define L293D_IN4 2

// EEPROM States
#define ON '1'
#define OFF '2'
/*****************/
#define Default_required_temperature 20 
/****************************************/
#define ON_AIR_CONDITIONER 'd'
#define OFF_AIR_CONDITIONER 'f'
#define SET_TEMP_MODE 'd'
#define ON_LED_ROOM1 'A'
#define OFF_LED_ROOM1 'B'
#define ON_FAN_ROOM1 'C'
#define OFF_FAN_ROOM1 'D'
#define OPEN_DOOR_ROOM1 'E'
#define CLOSE_DOOR_ROOM1 'F'
#define ON_LED_ROOM2 'G'
#define OFF_LED_ROOM2 'H'
#define ON_FAN_ROOM2 'I'
#define OFF_FAN_ROOM2 'a'
#define OPEN_DOOR_ROOM2 'b'
#define CLOSE_DOOR_ROOM2 'c'
#define ON_TV 'g'
#define OFF_TV 'h'
#define DISPLAY_STATUS_ON 'z'
#define DISPLAY_STATUS_OFF 'y'
/*********************/

#endif /* APP_CONFIG_H_ */