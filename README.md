# Arduino-Modbus-Floats
# March 2021 
Arduino Simple Modbus Float Slave 
This a simple script that will work with ATtiny85 to Arduino UNO.
It works very good.
At this time it has DS18B20 Temperature sensor and a LDR sensor for testing.
The unit also stores eeprom information like modbus slave address.
***Important that one load known values in the eeprom before loading the main script***
Refer to  script for actual setup.

Holding registers
***Integer
0-ADC1
1-ADC2
2-Temp DS18B20 Deg C
3 Temp DS18B20 Deg F
4- ADC1 m
5- ADC1 x
6- ADC1 y
7- ADC2 m
8- ADC2 x
9- ADC2 y
10-Slave Address
11-Baudrate
12-RS485 Control Pin (if used, if not 0)
***Floats for DS18B20 Sensor
13/14- Temp C
15/16- TEmp F
***Integer
17- Errors
18- Total Reg Size



