/*
 * Master.c
 *
 * Created: 10/6/2019 2:57:02 PM
 *  Author: Mariam Ashraf
 */ 

#include <avr/io.h>
#include "APP_config.h"
#include <util/delay.h>
#include "std_macros.h"
#include <avr/interrupt.h>
#include "USART.h"
#include "LCD.h"
#include "keypad.h"
#include "spi_driver.h"
#include "EEPROM.h"
char x=NOTPRESSED,y=NOTPRESSED;
char val = NOTPRESSED, i, flag = 0, try = MAX_TRIES;
unsigned char second=0,minutes=0,hours=0,first_digit,second_digit,temperture,first_digit_tem,second_digit_tem,option=NOTPRESSED;;

// Array to store entered password digits
char password[4];
int main(void)
{
	//initialization
	UART_vInit(9600);
	LCD_vInit();
	keypad_vInit();
	SPI_MasterInit();
	_delay_ms(500);
	
	
	LCD_vSend_string("   Welcome to");
	LCD_movecursor(2,1);
	LCD_vSend_string(" MY Smart home");
	_delay_ms(500);
	
	ResetPassword:
	// Check if the status indicates a new password needs to be set
	if (EEPROM_read(STATUS_LOCATION) == 0xff)
	{
		try=MAX_TRIES;
		LCD_clearscreen();
		LCD_vSend_string("Set Password of");
		LCD_movecursor(2, 1);
		LCD_vSend_string("4 digit:");

		// Loop to set a new 4-digit password
		for (i = 0; i < 4; i++)
		{
			
			if (option=='2')
			{
				val=UART_u8ReceiveData();
			}
			else 
			{
				do
				{
					val = keypad_u8check_press();
				} while (val == NOTPRESSED);
			}			
			LCD_vSend_char(val);
			_delay_ms(500);
			LCD_movecursor(2, 9 + i);
			LCD_vSend_char('*');
			EEPROM_write(PASSWORD_LOCATION1 + i, val);
		}

		// Mark the status as password set
		EEPROM_write(STATUS_LOCATION, 0x00);
	}
	

    while(1)
    {
		ChoiceWay:
		LCD_clearscreen();
		LCD_vSend_string(" choose option");
		_delay_ms(500);
		LCD_clearscreen();
		LCD_vSend_string("1-keypad.");
		LCD_movecursor(2,1);
		LCD_vSend_string("2-bluetooth.");
		if (option==NOTPRESSED||option=='1')
		{
			do 
			{
				option=keypad_u8check_press();
			} while (option==NOTPRESSED);
			_delay_ms(200);
		}
		else
		{
			option=UART_u8ReceiveData();
		}			

			CheckPassword:
			 LCD_clearscreen();

			 // Reset password entry array
			 password[0] = password[1] = password[2] = password[3] = NOTPRESSED;
			 LCD_vSend_string("Check Password:");
			 LCD_movecursor(2, 6);

			 // Loop to enter the password for opening the safe
			 for (i = 0; i < 4; i++)
			 {
				 if (option=='1')
				 {
					  do
					  {
						  password[i] = keypad_u8check_press();
					  } while (password[i] == NOTPRESSED);
					 
				 }
				 else
				 password[i]=UART_u8ReceiveData();
				

				 LCD_vSend_char(password[i]);
				 _delay_ms(300);
				 LCD_movecursor(2, 6 + i);
				 LCD_vSend_char('*');
			 }

			 // Check if the entered password matches
			 if (EEPROM_read(PASSWORD_LOCATION1) == password[0] &&
			 EEPROM_read(PASSWORD_LOCATION2) == password[1] &&
			 EEPROM_read(PASSWORD_LOCATION3) == password[2] &&
			 EEPROM_read(PASSWORD_LOCATION4) == password[3])
			 {
				 // Display success message and set the flag to open the safe
				 LCD_clearscreen();
				 LCD_vSend_string("right password");
				 LCD_movecursor(2,1);
				 LCD_vSend_string(" System opened");
				 _delay_ms(500);
				 flag = 1;
			 }
			 else
			 {
				 // Handle unsuccessful attempts to enter the correct password
				 try--;

				 if (try > 0)
				 {
					 // Display wrong password message and the number of remaining tries
					 LCD_clearscreen();
					 LCD_vSend_string(" wrong password");
					 _delay_ms(500);
					 LCD_clearscreen();
					 LCD_vSend_string(" You have only");
					 LCD_movecursor(2, 4);
					 LCD_vSend_char(try + 48);
					 LCD_vSend_string(" tries");
					 _delay_ms(500);
				 }
				 else
				 {
					 // Display wrong password message and wait for a certain time
					 LCD_clearscreen();
					 LCD_vSend_string(" wrong password");
					 _delay_ms(500);
					 LCD_clearscreen();
					 LCD_vSend_string("Try again after");
					 LCD_movecursor(2, 6);
					 LCD_vSend_string(" seconds");

					 // Countdown timer for waiting
					 for (i = 10; i > 0; i--)
					 {
						 LCD_movecursor(2, 4);
						 LCD_vSend_char((i / 10) + 48);
						 LCD_vSend_char((i % 10) + 48);
						 _delay_ms(500);
					 }
					 try=MAX_TRIES;
				 }
				 goto CheckPassword;
			 }
			FirstPage:
			LCD_clearscreen();
			LCD_vSend_string("1-Room1. 2-Room2");
			LCD_movecursor(2,1);
			LCD_vSend_string("3-AirConditioner");
			
			
			x=NOTPRESSED;
			Key_OR_Bluetooth();	
			if (x=='1')
			{
				ROOM1:
				LCD_clearscreen();
				LCD_vSend_string("1-Lamp.  2-Fan.");
				LCD_movecursor(2,1);
				LCD_vSend_string("3-Door.");
				
				x=NOTPRESSED;
				Key_OR_Bluetooth();
				if (x=='1')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-ON.  2-OFF.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						SPI_MasterTransmitchar(ON_LED_ROOM1,Slave1);
					}
					else if (x=='2')
					{
						SPI_MasterTransmitchar(OFF_LED_ROOM1,Slave1);
					}
					else if (x=='R') //Return
					{
						goto ROOM1;
					}
					 
				}
				else if (x=='2')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-ON.  2-OFF.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						SPI_MasterTransmitchar(ON_FAN_ROOM1,Slave1);
					}
					else if (x=='2')
					{
						SPI_MasterTransmitchar(OFF_FAN_ROOM1,Slave1);
					}
					else if (x=='R')
					{
						goto ROOM1;
					}
				}
				else if (x=='3')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-open.  2-close.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						SPI_MasterTransmitchar(OPEN_DOOR_ROOM1,Slave1);
					}
					else if (x=='2')
					{
						SPI_MasterTransmitchar(CLOSE_DOOR_ROOM1,Slave1);
					}
					else if (x=='R')
					{
						goto ROOM1;
					}
					
				}
				else if (x=='R') 
				{
					goto FirstPage;
				}
			}
			
			else if (x=='2')
			{
				ROOM2:
				LCD_clearscreen();
				LCD_vSend_string("1-Lamp.  2-Fan.");
				LCD_movecursor(2,1);
				LCD_vSend_string("3-Door.");
				
				x=NOTPRESSED;
				Key_OR_Bluetooth();
				if (x=='1')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-ON.  2-OFF.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						SPI_MasterTransmitchar(ON_LED_ROOM2,Slave1);
					}
					else if (x=='2')
					{
						SPI_MasterTransmitchar(OFF_LED_ROOM2,Slave1);
					}
					else if (x=='R')
					{
						goto ROOM2;
					}
					
				}
				else if (x=='2')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-ON.  2-OFF.");
					
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						SPI_MasterTransmitchar(ON_FAN_ROOM2,Slave1);
					}
					else if (x=='2')
					{
						SPI_MasterTransmitchar(OFF_FAN_ROOM2,Slave1);
					}
					else if (x=='R')
					{
						goto ROOM2;
					}
				}
				else if (x=='3')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-open.  2-close.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						SPI_MasterTransmitchar(OPEN_DOOR_ROOM2,Slave1);
					}
					else if (x=='2')
					{
						SPI_MasterTransmitchar(CLOSE_DOOR_ROOM2,Slave1);
					}
					else if (x=='R')
					{
						goto ROOM2;
					}
				}
				else if (x=='R')
				{
					goto FirstPage;
				}
				
			}
			else if (x=='3')
			{
				//air conditioner
				LCD_clearscreen();
				LCD_vSend_string("1-ON.  2-OFF.");
				x=NOTPRESSED;
				Key_OR_Bluetooth();
				if (x=='1')
				{
					LCD_clearscreen();
					LCD_vSend_string("Set Tem=--");
					LCD_movecursor(1,9);
					_delay_ms(500);
					first_digit=NOTPRESSED;
					Key_OR_Bluetooth_digit1();
					LCD_vSend_char(first_digit);
					_delay_ms(300);
					second_digit=NOTPRESSED;
					Key_OR_Bluetooth_digit2();
					LCD_vSend_char(second_digit);
					_delay_ms(300);
					
					temperture=(first_digit-48)*10+(second_digit-48);
					SPI_MasterTransmitchar(ON_AIR_CONDITIONER,Slave1);
					_delay_ms(300);
					SPI_MasterTransmitchar(temperture/10+48,Slave1);
					_delay_ms(300);
					SPI_MasterTransmitchar(temperture%10+48,Slave1);
				}
				else if (x=='2')
				{
					SPI_MasterTransmitchar(OFF_AIR_CONDITIONER,Slave1);
				}
				else if (x=='R')
				{
					goto FirstPage;
				}
			}
			else if (x=='M')
			{
				
				SecondPage:
				LCD_clearscreen();
				LCD_vSend_string("1-TV.  2-clock.");
				LCD_movecursor(2,1);
				LCD_vSend_string("3-Show status");
				x=NOTPRESSED;
				Key_OR_Bluetooth();
				if (x=='1')
				{
					//tv
					LCD_clearscreen();
					LCD_vSend_string("1-ON.  2-OFF.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						//on tv
						SPI_MasterTransmitchar(ON_TV,Slave1);
					}
					else if (x=='2')
					{
						//off tv
						SPI_MasterTransmitchar(OFF_TV,Slave1);
					}
					else if (x=='R')
					{
						goto SecondPage;
					}
				}
				else if (x=='2')
				{
					//clock
					LCD_clearscreen();
					LCD_vSend_string("1-set clock.");
					LCD_movecursor(2,1);
					LCD_vSend_string("2-set alarm");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{

						SPI_MasterTransmitchar(SetClock,Slave2);
						SetClockHour:
						LCD_clearscreen();
						LCD_vSend_string("Hours=--");
						LCD_movecursor(1,7);
						_delay_ms(500);
						first_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit1();						
						LCD_vSend_char(first_digit);
						_delay_ms(300);
						second_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit2();
						LCD_vSend_char(second_digit);
						_delay_ms(300);
						
						hours=(first_digit-48)*10+(second_digit-48);
						if (hours>24)
						{
							LCD_clearscreen();
							LCD_vSend_string("Error Hours are");
							LCD_movecursor(2,1);
							LCD_vSend_string("between 0 and 24");
							_delay_ms(1000);
							LCD_clearscreen();
							LCD_vSend_string("   Try again");
							_delay_ms(500);
							goto SetClockHour;
						}
						SPI_MasterTransmitchar(hours/10+48,Slave2);
						_delay_ms(300);
						SPI_MasterTransmitchar(hours%10+48,Slave2);
						SetClockMinutes:
						LCD_clearscreen();
						LCD_vSend_string("minutes=--");
						LCD_movecursor(1,9);
						_delay_ms(500);
						first_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit1();
						LCD_vSend_char(first_digit);
						_delay_ms(300);
						second_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit2();
						LCD_vSend_char(second_digit);
						_delay_ms(300);
						minutes=(first_digit-48)*10+(second_digit-48);
						if (minutes>60)
						{
							LCD_clearscreen();
							LCD_vSend_string("Error min are");
							LCD_movecursor(2,1);
							LCD_vSend_string("between 0 and 60");
							_delay_ms(1000);
							LCD_clearscreen();
							LCD_vSend_string("   Try again");
							_delay_ms(500);
							goto SetClockMinutes;
						}
						SPI_MasterTransmitchar(minutes/10+48,Slave2);
						_delay_ms(300);
						SPI_MasterTransmitchar(minutes%10+48,Slave2);
						SetClockSeconds:
						LCD_clearscreen();
						LCD_vSend_string("seconds=--");
						LCD_movecursor(1,9);
						_delay_ms(500);
						first_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit1();
						LCD_vSend_char(first_digit);
						_delay_ms(300);
						second_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit2();
						LCD_vSend_char(second_digit);
						_delay_ms(300);
						second=(first_digit-48)*10+(second_digit-48);
						if (second>60)
						{
							LCD_clearscreen();
							LCD_vSend_string("Error sec are");
							LCD_movecursor(2,1);
							LCD_vSend_string("between 0 and 60");
							_delay_ms(1000);
							LCD_clearscreen();
							LCD_vSend_string("   Try again");
							_delay_ms(500);
							goto SetClockSeconds;
						}
						SPI_MasterTransmitchar(second/10+48,Slave2);
						_delay_ms(300);
						SPI_MasterTransmitchar(second%10+48,Slave2);
						
					}

					else if (x=='2')
					{
						SPI_MasterTransmitchar(SetAlarm,Slave2);
						SetAlarmHours:
						LCD_clearscreen();
						LCD_vSend_string("Hours=--");
						LCD_movecursor(1,7);
						_delay_ms(500);
						first_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit1();
						LCD_vSend_char(first_digit);
						_delay_ms(300);
						second_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit2();
						LCD_vSend_char(second_digit);
						_delay_ms(300);
						
						hours=(first_digit-48)*10+(second_digit-48);
						if (hours>24)
						{
							LCD_clearscreen();
							LCD_vSend_string("Error Hours are");
							LCD_movecursor(2,1);
							LCD_vSend_string("between 0 and 24");
							_delay_ms(1000);
							LCD_clearscreen();
							LCD_vSend_string("   Try again");
							_delay_ms(500);
							goto SetAlarmHours;
						}
						SPI_MasterTransmitchar((hours/10)+48,Slave2);
						_delay_ms(300);
						SPI_MasterTransmitchar((hours%10)+48,Slave2);
						SetAlarmMinutes:
						LCD_clearscreen();
						LCD_vSend_string("minutes=--");
						LCD_movecursor(1,9);
						_delay_ms(500);
						first_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit1();
						LCD_vSend_char(first_digit);
						_delay_ms(300);
						second_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit2();
						LCD_vSend_char(second_digit);
						_delay_ms(300);
						minutes=(first_digit-48)*10+(second_digit-48);
						if (minutes>60)
						{
							LCD_clearscreen();
							LCD_vSend_string("Error min are");
							LCD_movecursor(2,1);
							LCD_vSend_string("between 0 and 60");
							_delay_ms(1000);
							LCD_clearscreen();
							LCD_vSend_string("   Try again");
							_delay_ms(500);
							goto SetAlarmMinutes;
						}
						SPI_MasterTransmitchar((minutes/10)+48,Slave2);
						_delay_ms(300);
						SPI_MasterTransmitchar((minutes%10)+48,Slave2);
						SetAlarmSeconds:
						LCD_clearscreen();
						LCD_vSend_string("seconds=--");
						LCD_movecursor(1,9);
						_delay_ms(500);
						first_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit1();
						LCD_vSend_char(first_digit);
						_delay_ms(300);
						second_digit=NOTPRESSED;
						Key_OR_Bluetooth_digit2();
						LCD_vSend_char(second_digit);
						_delay_ms(300);
						second=(first_digit-48)*10+(second_digit-48);
						if (second>60)
						{
							LCD_clearscreen();
							LCD_vSend_string("Error sec are");
							LCD_movecursor(2,1);
							LCD_vSend_string("between 0 and 60");
							_delay_ms(1000);
							LCD_clearscreen();
							LCD_vSend_string("   Try again");
							_delay_ms(500);
							goto SetAlarmSeconds;
						}
						SPI_MasterTransmitchar((second/10)+48,Slave2);
						_delay_ms(300);
						SPI_MasterTransmitchar((second%10)+48,Slave2);
					}
					else if (x=='R')
					{
						goto SecondPage;
					}
				}
				else if (x=='3')
				{
					//show status
					LCD_clearscreen();
					LCD_vSend_string("1-Show.  2-Hide.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						//show statues
						SPI_MasterTransmitchar(DISPLAY_STATUS_ON,Slave1);
					}
					else if (x=='2')
					{
						//hide statues
						SPI_MasterTransmitchar(DISPLAY_STATUS_OFF,Slave1);
					}
					else if (x=='R')
					{
						goto SecondPage;
					}
					
				}
				else if (x=='R')
				{
					goto FirstPage;
				}
				else if (x=='M')
				{
					LCD_clearscreen();
					LCD_vSend_string("1-Edit password.");
					LCD_movecursor(2,1);
					LCD_vSend_string("2-Edit option.");
					x=NOTPRESSED;
					Key_OR_Bluetooth();
					if (x=='1')
					{
						LCD_clearscreen();
						password[0] = password[1] = password[2] = password[3] = NOTPRESSED;

						LCD_vSend_string("Check old Pass:");
						LCD_movecursor(2, 6);

						// Loop to enter the old password for reset
						for (i = 0; i < 4; i++)
						{
							if (option=='1')
							{	do
								{
									password[i] = keypad_u8check_press();
								} while (password[i] == NOTPRESSED);
							}
							else
							{
								password[i]=UART_u8ReceiveData();
							}							
							LCD_vSend_char(password[i]);
							_delay_ms(500);
							LCD_movecursor(2, 6 + i);
							LCD_vSend_char('*');
						}

						// Check if the entered old password matches
						if (EEPROM_read(PASSWORD_LOCATION1) == password[0] &&
						EEPROM_read(PASSWORD_LOCATION2) == password[1] &&
						EEPROM_read(PASSWORD_LOCATION3) == password[2] &&
						EEPROM_read(PASSWORD_LOCATION4) == password[3])
						{
							// Mark the status as password reset and go back to setting a new password
							EEPROM_write(STATUS_LOCATION, 0xff);
							goto ResetPassword;
						}
						else
						{
							// Display wrong password message and wait for a certain time
							LCD_clearscreen();
							LCD_vSend_string(" wrong password");
							_delay_ms(500);
							LCD_clearscreen();
							LCD_vSend_string("Try again after");
							LCD_movecursor(2, 6);
							LCD_vSend_string(" seconds");

							// Countdown timer for waiting
							for (i = 10; i > 0; i--)
							{
								LCD_movecursor(2, 4);
								LCD_vSend_char((i / 10) + 48);
								LCD_vSend_char((i % 10) + 48);
								_delay_ms(500);
							}
						}
						//EEPROM_write(STATUS_LOCATION, 0xff);
						goto CheckPassword; 
					}
					else if (x=='2')
					{
						goto ChoiceWay;
					}
					else if (x=='R')
					{
						goto SecondPage;
					}
					
					
				}
				
			}
			goto FirstPage;
		}
}
void Key_OR_Bluetooth(void)
{
	if (option=='1')
	{
		do
		{
			x=keypad_u8check_press();
		} while (x==NOTPRESSED);
		_delay_ms(200);
	}
	else
	{
		x=UART_u8ReceiveData();
	}
}
void Key_OR_Bluetooth_digit1(void)
{
	
	if (option=='1')
	{
		do
		{
			first_digit=keypad_u8check_press();
		
		} while (first_digit==NOTPRESSED);
		_delay_ms(200);
	}
	else
	{
		first_digit=UART_u8ReceiveData();
	}
}
void Key_OR_Bluetooth_digit2(void)
{
	if (option=='1')
	{
		do
		{
			second_digit_tem=keypad_u8check_press();
			
		} while (second_digit==NOTPRESSED);
		_delay_ms(200);
	}
	else
	{
		second_digit=UART_u8ReceiveData();
	}
}