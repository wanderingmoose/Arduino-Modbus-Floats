# Arduino-Modbus-Floats
# March 2021 
Arduino Simple Modbus Float Slave </br> 
This a simple script that will work with ATtiny85 to Arduino UNO.</br>
It works very good.</br>
At this time it has DS18B20 Temperature sensor and a LDR sensor for testing.</br>
The unit also stores eeprom information like modbus slave address.</br>
***Important that one load known values in the eeprom before loading the main script***</br>
The above statement is for UNO only, but you will find with loading in ATtiny85 using ATmel programmer, the eeprom is cleared and the the Slave address will go to 255.</br>
Refer to  script for actual setup.</br>
</br>
Holding registers</br>
***Integer</br>
0-ADC1</br>
1-ADC2</br>
2-Temp DS18B20 Deg C</br>
3 Temp DS18B20 Deg F</br>
4- ADC1 m</br>
5- ADC1 x</br>
6- ADC1 y</br>
7- ADC2 m</br>
8- ADC2 x</br>
9- ADC2 y</br>
10-Slave Address</br>
11-Baudrate</br>
12-RS485 Control Pin (if used, if not 0)</br>
***Floats for DS18B20 Sensor</br>
13/14- Temp C</br>
15/16- TEmp F</br>
***Integer</br>
17- Errors</br>
18- Total Reg Size</br>



