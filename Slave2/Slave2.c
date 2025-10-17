/*
 * Slave2.c
 *
 * Created: 11/5/2024 6:49:21 PM
 *  Author: Mariam Ashraf
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "APP_config.h"
#include <util/delay.h>
#include "DIO.h"
#include "LED.h"
#include "button.h"
#include "ADC_driver.h"
#include "Timer.h"
#include "spi_driver.h"
#include "sev_seg.h"

unsigned long LDR_res;
volatile unsigned char x;
unsigned short temperature, LDR, Analog_Signal_LDR, Analog_Signal_Smoke;
volatile unsigned char sec_counter = 0;
volatile unsigned char minutes = 0, hours = 0 , ALARMhours = -1, ALARMminutes = -1, ALARMsec_counter = -1;
volatile signed char  counter = 0, counter2 = 0, counter3 = 0, counter4 = 0, counter_temp = 0, counter_temp1 = 0;

int main(void)
{
    LED_vInit(BUZZER_ALARM_PORT, BUZZER_ALARM_PIN); // Buzzer for alarm clock
    LED_vInit(BUZZER_SOMKE_PORT, BUZZER_SMOKE_PIN); // Buzzer for smoke sensor
    LED_vInit(LDR_LED1_PORT, LDR_LED1_PIN1);  // LDR LED1
    LED_vInit(LDR_LED2_PORT, LDR_LED2_PIN2); // LDR LED2
    
    // Initialize button for alarm control
    button_vInit(BUTTON_PORT, BUTTON_PIN);
    
    // Enable global interrupts
    sei();
    
    // Initialize timer2 for overflow interrupt
    timer2_overflow_init_interrupt();

    // Set pin directions for 7-segment display control
    DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
    DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
    DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
    DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
    DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
    DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
    DIO_set_port_direction(SEVEN_SEG_PORT, 1);  // Set SEVEN_SEG_PORT as output

    // Enable the 7-segment display digits
    DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
    DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
    DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
    DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
    DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
    DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);

    // Initialize variables for timekeeping
    sec_counter = 0;
    minutes = 0, hours = 0;
    
    // Initialize ADC and SPI
    ADC_vinit();
    SPI_SlaveInit();
    
    // Initialize 7-segment display
    seven_seg_vinit(SEVEN_SEG_PORT);
    
    // Enable SPI interrupt
    SPCR |= (1 << SPIE);
    
    while (1)
    {
        if (counter == 0)
        {
            // Display seconds on 7-segment display
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 0);
            seven_seg_write(SEVEN_SEG_PORT, sec_counter % 10);
            _delay_ms(2);

            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 0);
            seven_seg_write(SEVEN_SEG_PORT, sec_counter / 10);
            _delay_ms(2);

            // Display minutes on 7-segment display
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 0);
            seven_seg_write(SEVEN_SEG_PORT, minutes % 10);
            _delay_ms(2);

            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 0);
            seven_seg_write(SEVEN_SEG_PORT, minutes / 10);
            _delay_ms(2);

            // Display hours on 7-segment display
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 0);
            seven_seg_write(SEVEN_SEG_PORT, hours % 10);
            _delay_ms(2);

            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
            DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 0);
            seven_seg_write(SEVEN_SEG_PORT, hours / 10);
            _delay_ms(2);

            // Increment time variables
            if (sec_counter == 60)
            {
                sec_counter = 0;
                minutes++;
            }
            if (minutes == 60)
            {
                minutes = 0;
                hours++;
            }
            if (hours == 24)
            {
                hours = 0;
            }
        }

        // Reset the last 7-segment digit to prevent ghosting
        DIO_write(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);

        // Check if the current time matches the alarm time
        if (ALARMhours == hours && ALARMminutes == minutes && ALARMsec_counter == sec_counter)
        {
            // Turn on the alarm buzzer
            LED_vTurnOn(BUZZER_ALARM_PORT, BUZZER_ALARM_PIN);
        }

        // Check if the button is pressed to turn off the alarm
        if (button_u8read(BUTTON_PORT, BUTTON_PIN) == 1)
        {
            // Turn off the alarm buzzer
            LED_vTurnOff(BUZZER_ALARM_PORT, BUZZER_ALARM_PIN);
            ALARMhours = -1;
            ALARMminutes = -1;
            ALARMsec_counter = -1;
        }

        // Read and process LDR sensor data
        LDR = ADC_u16Read(LDR_CHANNEL);
        LDR = (LDR * 5000UL) / 1024;
        LDR_res = LDR / 10;
        if (LDR_res >= 490)
        {
            // Turn on both LEDs
            LED_vTurnOn(LDR_LED1_PORT, LDR_LED1_PIN1);
            LED_vTurnOn(LDR_LED2_PORT, LDR_LED2_PIN2);
        }
        else if (LDR_res >= 480)
        {
            // Turn on one LED
            LED_vTurnOn(LDR_LED2_PORT, LDR_LED2_PIN2);
            LED_vTurnOff(LDR_LED1_PORT, LDR_LED1_PIN1);
        }
        else
        {
            // Turn off both LEDs
            LED_vTurnOff(LDR_LED1_PORT, LDR_LED1_PIN1);
            LED_vTurnOff(LDR_LED2_PORT, LDR_LED2_PIN2);
        }

        // Read and process smoke sensor data
        Analog_Signal_Smoke = ADC_u16Read(SMOKE_CHANNEL);
        Analog_Signal_Smoke = ((Analog_Signal_Smoke * 500UL) / 1024) / Factor;

        // Check if the smoke level exceeds the threshold
        if (Analog_Signal_Smoke >= default_Value_Smoke)
        {
            // Turn on the smoke alarm buzzer
            LED_vTurnOn(BUZZER_SOMKE_PORT, BUZZER_SMOKE_PIN);
        }
        else
        {
            // Turn off the smoke alarm buzzer
            LED_vTurnOff(BUZZER_SOMKE_PORT, BUZZER_SMOKE_PIN);
        }
    }
}

// SPI interrupt service routine for receiving data
ISR(SPI_STC_vect)
{
    x = SPI_SlaveReceivechar(DUMMY_DATA);
	// This part of the code prevents the slave from making any action on the clock or alarm when the master is setting it and sending data to the slave.
	if (counter > 0)
	counter2++; // If the clock setting process has started, increment counter2
	if (counter3 > 0)
	counter4++; // If the alarm setting process has started, increment counter4

	// Check if the received character indicates the start of clock setting
	if (x == SetClock)
	{
		counter++; // Increment counter to indicate the clock setting process has started
		
		// Enable all the pins of the 7-segment display for clock setting
		DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN0, 1);
		DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN1, 1);
		DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN2, 1);
		DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT1, SEVEN_SEG_ENABLE_PIN3, 1);
		DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN4, 1);
		DIO_vsetPINDir(SEVEN_SEG_ENABLE_PORT2, SEVEN_SEG_ENABLE_PIN5, 1);
	}

	// Set hours based on the received data
	if (counter2 == 1)
	{
		hours = (x - 48) * 10; // Set the tens digit of hours
	}
	if (counter2 == 2)
	{
		hours += (x - 48) % 10; // Set the units digit of hours
	}

	// Set minutes based on the received data
	if (counter2 == 3)
	{
		minutes = (x - 48) * 10; // Set the tens digit of minutes
	}
	if (counter2 == 4)
	{
		minutes += (x - 48) % 10; // Set the units digit of minutes
	}

	// Set seconds based on the received data
	if (counter2 == 5)
	{
		sec_counter = (x - 48) * 10; // Set the tens digit of seconds
	}
	if (counter2 == 6)
	{
		sec_counter += (x - 48) % 10; // Set the units digit of seconds
		counter = 0; // Reset counter to indicate the end of clock setting process
		counter2 = 0; // Reset counter2 for future use
	}

	// Check if the received character indicates the start of alarm setting
	if (x == SetAlarm)
	{
		counter3++; // Increment counter3 to indicate the alarm setting process has started
	}

	// Set alarm hours based on the received data
	if (counter4 == 1)
	{
		ALARMhours = (x - 48) * 10; // Set the tens digit of alarm hours
	}
	if (counter4 == 2)
	{
		ALARMhours += (x - 48) % 10; // Set the units digit of alarm hours
	}

	// Set alarm minutes based on the received data
	if (counter4 == 3)
	{
		ALARMminutes = (x - 48) * 10; // Set the tens digit of alarm minutes
	}
	if (counter4 == 4)
	{
		ALARMminutes += (x - 48) % 10; // Set the units digit of alarm minutes
	}

	// Set alarm seconds based on the received data
	if (counter4 == 5)
	{
		ALARMsec_counter = (x - 48) * 10; // Set the tens digit of alarm seconds
	}
	if (counter4 == 6)
	{
		ALARMsec_counter += (x - 48) % 10; // Set the units digit of alarm seconds
		counter3 = 0; // Reset counter3 to indicate the end of alarm setting process
		counter4 = 0; // Reset counter4 for future use
	}
}	
// Timer2 overflow interrupt service routine for updating the second counter
ISR(TIMER2_OVF_vect)
{
    sec_counter++;
}
