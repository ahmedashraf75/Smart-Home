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
/************************/
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
#define SetClock 'j'
#define SetAlarm 'k'
/*****************/
// Define EEPROM locations and maximum number of password tries
#define STATUS_LOCATION     0x30
#define PASSWORD_LOCATION1  0x31
#define PASSWORD_LOCATION2  0x32
#define PASSWORD_LOCATION3  0x33
#define PASSWORD_LOCATION4  0x34
#define MAX_TRIES 3
/***********************/
#define Slave1 0
#define Slave2 1

void Key_OR_Bluetooth(void);
void Key_OR_Bluetooth_digit1(void);
void Key_OR_Bluetooth_digit2(void);

#endif /* APP_CONFIG_H_ */