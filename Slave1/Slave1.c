/*
 * Slave1.c
 *
 * Created: 4/10/2024 12:22:00 PM
 *  Author: Mariam Ashraf
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "APP_config.h"
#include <util/delay.h>
#include "spi_driver.h"
#include "DIO.h"
#include "LCD.h"
#include "LED.h"
#include "Timer.h"
#include "EEPROM.h"
#include "ADC_driver.h"

unsigned char temp_flag=1,statues=0,temperature,last_air_conditioning_value;
volatile unsigned char x,required_temperature=Default_required_temperature,DisplayCounter=0,display=0,counter_temp=0,counter_temp1=0,counter=0,counter2=0,counter3=0,counter4=0;

int main(void)
{
    // Initialize LEDs and other devices
    LED_vInit(LED_PORT, LED_PIN1);
    LED_vInit(LED_PORT, LED_PIN2);
    LED_vInit(AIR_CONDITIONER_PORT, AIR_CONDITIONER_PIN);
    DIO_vsetPINDir(FAN_PORT, FAN_PIN1, 1);
    DIO_vsetPINDir(FAN_PORT, FAN_PIN2, 1);
    DIO_vsetPINDir(TV_PORT, TV_PIN, 1); // TV
    DIO_vsetPINDir(L293D_PORT, L293D_IN2, 1); // IN2 L293D
    DIO_vsetPINDir(L293D_PORT, L293D_EN, 1); // enable
    DIO_vsetPINDir(L293D_PORT, L293D_IN4, 1); // IN4 L293D
    DIO_write(L293D_PORT, L293D_EN, 1); // enable L293D
    
    // Initiation
    ADC_vinit();
    LCD_vInit();
    SPI_SlaveInit();
    timer_CTC_init_interrupt();
    SET_BIT(SPCR, SPIE); // Enable SPI interrupt
    sei(); // Enable global interrupts
    
    while(1)
    {
        // Air conditioner control
        if (x == ON_AIR_CONDITIONER || EEPROM_read(AIR_LOCATION1) == ON) {
            EEPROM_write(AIR_LOCATION1, ON);
            temp_flag = 0;
        }
        if (x == OFF_AIR_CONDITIONER || EEPROM_read(AIR_LOCATION1) == OFF) {
            EEPROM_write(AIR_LOCATION1, OFF);
            LED_vTurnOff(AIR_CONDITIONER_PORT, AIR_CONDITIONER_PIN);
            temp_flag = 1;
        }
        
        // Read temperature from ADC
        temperature = (ADC_u16Read(ADC_CHANNEL) * 0.25);
        
        // Air conditioner logic based on temperature
        if (counter_temp == 0 && temp_flag == 0) {
            if (temperature >= required_temperature + 1) {
                LED_vTurnOn(AIR_CONDITIONER_PORT, AIR_CONDITIONER_PIN);
                last_air_conditioning_value = 1;
            } else if (temperature <= required_temperature - 1) {
                LED_vTurnOff(AIR_CONDITIONER_PORT, AIR_CONDITIONER_PIN);
                last_air_conditioning_value = 0;
            } else if (required_temperature == temperature) {
                if (last_air_conditioning_value == 1) {
                    LED_vTurnOn(AIR_CONDITIONER_PORT, AIR_CONDITIONER_PIN);
                } else if (last_air_conditioning_value == 0) {
                    LED_vTurnOff(AIR_CONDITIONER_PORT, AIR_CONDITIONER_PIN);
                }
            }
        }
        
        // LED room 1 control
        if (x == ON_LED_ROOM1 || EEPROM_read(LED_LOCATION1) == ON) {
            EEPROM_write(LED_LOCATION1, ON);
            LED_vTurnOn(LED_PORT, LED_PIN1);
        }
        if (x == OFF_LED_ROOM1 || EEPROM_read(LED_LOCATION1) == OFF) {
            EEPROM_write(LED_LOCATION1, OFF);
            LED_vTurnOff(LED_PORT, LED_PIN1);
        }
        
        // Fan room 1 control
        if (x == ON_FAN_ROOM1 || EEPROM_read(FAN_LOCATION1) == ON) {
            EEPROM_write(FAN_LOCATION1, ON);
            DIO_write(FAN_PORT, FAN_PIN1, 1);
        }
        if (x == OFF_FAN_ROOM1 || EEPROM_read(FAN_LOCATION1) == OFF) {
            EEPROM_write(FAN_LOCATION1, OFF);
            DIO_write(FAN_PORT, FAN_PIN1, 0);
        }
        
        // Door room 1 control
        if (x == OPEN_DOOR_ROOM1 || EEPROM_read(DOOR_LOCATION1) == ON) {
            EEPROM_write(DOOR_LOCATION1, ON);
            timer1_wave_fastPWM_B(DOOR_TIMER1_WAVE_open);
        }
        if (x == CLOSE_DOOR_ROOM1 || EEPROM_read(DOOR_LOCATION1) == OFF) {
            EEPROM_write(DOOR_LOCATION1, OFF);
            timer1_wave_fastPWM_B(DOOR_TIMER1_WAVE_close);
        }
        
        // LED room 2 control
        if (x == ON_LED_ROOM2 || EEPROM_read(LED_LOCATION2) == ON) {
            EEPROM_write(LED_LOCATION2, ON);
            LED_vTurnOn(LED_PORT, LED_PIN2);
        }
        if (x == OFF_LED_ROOM2 || EEPROM_read(LED_LOCATION2) == OFF) {
            EEPROM_write(LED_LOCATION2, OFF);
            LED_vTurnOff(LED_PORT, LED_PIN2);
        }
        
        // Fan room 2 control
        if (x == ON_FAN_ROOM2 || EEPROM_read(FAN_LOCATION2) == ON) {
            EEPROM_write(FAN_LOCATION2, ON);
            DIO_write(FAN_PORT, FAN_PIN2, 1);
        }
        if (x == OFF_FAN_ROOM2 || EEPROM_read(FAN_LOCATION2) == OFF) {
            EEPROM_write(FAN_LOCATION2, OFF);
            DIO_write(FAN_PORT, FAN_PIN2, 0);
        }
        
        // Door room 2 control
        if (x == OPEN_DOOR_ROOM2 || EEPROM_read(DOOR_LOCATION2) == ON) {
            EEPROM_write(DOOR_LOCATION2, ON);
            timer1_wave_fastPWM_A(DOOR_TIMER1_WAVE_open);
        }
        if (x == CLOSE_DOOR_ROOM2 || EEPROM_read(DOOR_LOCATION2) == OFF) {
            EEPROM_write(DOOR_LOCATION2, OFF);
            timer1_wave_fastPWM_A(DOOR_TIMER1_WAVE_close);
        }
        
        // TV control
        if (x == ON_TV || EEPROM_read(TV_LOCATION) == ON) {
            EEPROM_write(TV_LOCATION, ON);
            DIO_write(TV_PORT, TV_PIN, 1);
        }
        if (x == OFF_TV || EEPROM_read(TV_LOCATION) == OFF) {
            EEPROM_write(TV_LOCATION, OFF);
            DIO_write(TV_PORT, TV_PIN, 0);
        }

        // Display status on LCD
        if (x == DISPLAY_STATUS_ON || EEPROM_read(STATUES_LOCATION) == ON) {
            EEPROM_write(STATUES_LOCATION, ON);
            if (display == 0) {
                // Display room 1 status
                LCD_movecursor(1, 1);
                LCD_vSend_string("R1 (Door-");
                if (EEPROM_read(DOOR_LOCATION1) == ON)
                    LCD_vSend_string("opened");
                else
                    LCD_vSend_string("closed,");
                LCD_movecursor(2, 1);
                LCD_vSend_string("Led-");
                if (EEPROM_read(LED_LOCATION1) == ON)
                    LCD_vSend_string("ON ");
                else
                    LCD_vSend_string("Off");
                LCD_vSend_string(",Fan-");
                if (EEPROM_read(FAN_LOCATION1) == ON)
                    LCD_vSend_string("ON )");
                else
                    LCD_vSend_string("Off)");
            } else if (display == 2) {
                // Display room 2 status
                LCD_movecursor(1, 1);
                LCD_vSend_string("R2 (Door-");
                if (EEPROM_read(DOOR_LOCATION2) == ON)
                    LCD_vSend_string("opened");
                else
                    LCD_vSend_string("closed,");
                LCD_movecursor(2, 1);
                LCD_vSend_string("Led-");
                if (EEPROM_read(LED_LOCATION2) == ON)
                    LCD_vSend_string("ON ");
                else
                    LCD_vSend_string("Off");
                LCD_vSend_string(",Fan-");
                if (EEPROM_read(FAN_LOCATION2) == ON)
                    LCD_vSend_string("ON )");
                else
                    LCD_vSend_string("Off)");
            } else if (display == 4) {
                // Display air conditioner status
                LCD_movecursor(1, 1);
                LCD_vSend_string(" AirConditioner ");
                LCD_movecursor(2, 1);
                if (EEPROM_read(AIR_LOCATION1) == ON) {
                    if (READ_BIT(PORTA, AIR_CONDITIONER_PIN) == 1)
                        LCD_vSend_string("      (ON)      ");
                    else
                        LCD_vSend_string("   (ON-Idle)    ");
                } else
                    LCD_vSend_string("      (OFF)     ");
            } else if (display == 6) {
                // Display TV status
                LCD_movecursor(2, 1);
                LCD_vSend_string("                ");
                LCD_movecursor(1, 1);
                LCD_vSend_string("    TV-");
                if (EEPROM_read(TV_LOCATION) == ON)
                    LCD_vSend_string("(ON)     ");
                else
                    LCD_vSend_string("(OFF)    ");
            }
        }
        if (x == DISPLAY_STATUS_OFF || EEPROM_read(STATUES_LOCATION) == OFF) {
            EEPROM_write(STATUES_LOCATION, OFF);
            LCD_clearscreen();
            DisplayCounter = 0;
        }
    }
}

ISR(SPI_STC_vect) {
    x = SPI_SlaveReceivechar(DUMMY_DATA);
	// This Logic of the code prevents the slave from making any action on by part of required_temperature by temperature when the master is setting it and sending data to the slave.
    if (counter_temp > 0)
        counter_temp1++;
    if (x == SET_TEMP_MODE) {
        counter_temp++;
    }
    if (counter_temp1 == 1) {
        required_temperature = (x - 48) * 10;
    }
    if (counter_temp1 == 2) {
        required_temperature += (x - 48) % 10;
        counter_temp = 0;
        counter_temp1 = 0;
    }
}

ISR(TIMER0_COMP_vect) {
    DisplayCounter++;
    if (DisplayCounter >= 100) {
        display++;
        DisplayCounter = 0;
    }
    if (display == 8)
        display = 0;
}