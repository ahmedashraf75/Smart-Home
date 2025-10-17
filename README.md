## Smart-Home
Using Bluetooth 🌟 Smart Home System Utilizing 3 AVR Microcontrollers with USART, SPI, and Bluetooth Features 🌟 🔧 Components:

1. 3 ATmega32 Microcontrollers
2. Bluetooth Module HC-05
3. 2 LCD16x2 Displays
4. Keypad
5. 2 Servo Motors
6. 2 Buzzers
7. 4 DC Motors
8. LM35 Temperature Sensor
9. MPX4115 Smoke Sensor
10. LEDs
11. LDR Sensor
12. 6 Seven Segment Displays


🧩 Software Architecture Layers:

1. Application Layer: main.c
2. Drivers: LED, LCD, Keypad, EEPROM, DIO, ADC, Seven Segment, Timers, USART, SPI
   
🚀 System Features: 🎛 User Interface:

1. Control Options: Users can control the home either wirelessly through Bluetooth using the HC-05 module or via the wired keypad.

🔐 Security System: 2. Password-Protected Access: Users have a password saved in the internal EEPROM at system startup. The user has three attempts to enter the correct password. After three incorrect attempts, the system will initiate a countdown and retry. Upon entering the correct password, the system will activate.

🔧 Controlling: 3. The master displays all available systems on the LCD (e.g., TV, Rooms, Air Conditioner). The user selects one and chooses to turn it on or off. The master sends this data to the slave, which executes the corresponding functions to control the target system. Each room contains lamps, fans, and doors, along with TVs and air conditioners. The air conditioner's operation is determined by the temperature sensor; if the temperature exceeds the user-set value, the air conditioner turns on, otherwise, it remains in idle mode.

📊 Status Display: 4. Users can request to view the status of various systems. The master sends the request to the slave, which displays the status of both rooms, TVs, and air conditioners on the LCD.

🌞 Outdoor Light Control: 5. The system reads the LDR sensor to determine outdoor lighting conditions. If sunlight is intense, two LEDs are turned off. If light is moderate, one LED is turned on. If light is low, the system turns on two LEDs.

🚨 Fire Alarm System: 6. The system monitors the smoke sensor. If the reading exceeds 50, it activates the water pump and buzzer.

⏰ Clock and Alarm Setting: 7. Users can set the clock and alarm. The master receives input from the user, sends it to the slave, and displays the clock on the seven-segment displays. If the alarm time matches the clock, the buzzer is activated.

🔑 Edit Password: 8. Users can change their password. The master prompts for the old and new passwords and handles the update by saving the new password to the internal EEPROM memory to preserve it during power loss.

🔄 Edit Control Option: 9. Users can switch control options between keypad and Bluetooth.

💾 Saving Status: 10. The status of the TV, rooms, and air conditioner is saved in EEPROM, ensuring that if power is lost and restored, the previous status is retained.
