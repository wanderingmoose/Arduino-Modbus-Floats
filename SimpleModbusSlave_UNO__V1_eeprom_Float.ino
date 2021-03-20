/*
 * ****Waring*************
 * When programming the eeprom will go to 255, so the starting modbus slave address will be 255!!!
 * once changed changed it will stay to what you set it to.
 * ***********************
  This is a Arduino Modbus script that reads DS18B20 and two analogs for ATtiny85
  Built around the SimpleModbusSlaveSoftwareSerial library.
  What did I learn, int is a signed 16 bit integer.
  When using 16 bit int it will give a negetive number for modbus.
   Largest int16 number if unsigned is 65535.
  Largest 1nt16 number if signed is 32787 positive. Negitive -32768 using 2'complemt.
  So one is limited to how big a number can be.
****
****
***************
  March 13, 2021 Starting to create this script
  So far found ADC1 works, but ADC2 not so much.
***************
  This version will also allow for changing the msa and store it for next reboot. This will allow setup of modules without reloading software.
  Setting up multipliers and adders could also be setup.
  would use 0-3 for int readings.
  4-9 for ranging of sensors. Int values only
  10-12 for system info..
  13-16 for Float information of temperature. Yes you read correctly, FLOATS!!!!!
  Dam this is great!!!!
 Used Tom Tobback's demo for slicing floats into unsigned integers.
 * INDUSTRUINO ModBus help (D21G)
 * modbus registers are 16-bit unsigned integers
 * to pass a float variable (32-bit), it needs to be split into 2 registers
 * use the below functions to achieve this
 * Tom Tobback Nov 2017
 
****
****
  One must prep the EEPROM using a seperate ino to clean and write basic startup data to the eeprom.
  Thinking this will be called SimpleModbusSlave_EEPROM_PREP_V1.ino
  This will give the unit a address of 1.
****
*Integers
  0 and 1 will be for TX/RX.
  2 will be for DS18B20 sensor
  3 will be for ADC input. VIN when we are done.
  4 will be for ADC input, 0-5 volts
  Floats (Modicon)
  13-14 Temperature C
  15-16 Temperature F
*/
//#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

// Data wire is plugged into port 0 on the ATTiny
#define ONE_WIRE_BUS 2 // pin for communications.

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//////////////// registers of your slave ///////////////////
enum {
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  value3,      //0 Vin
  value4,     //1 Spare
  TEMP_C,     //2 Int
  TEMP_F,     //3 Int
  ADC3m,       //4
  ADC3x,       //5
  ADC3y,       //6
  ADC4m,       //7
  ADC4x,       //8
  ADC4y,       //9
  SLAVEADDRESS,//10
  BAUDRATE,   //11
  RS485CTRLPIN,//12
  TempCLB,    //13  FloatA Modicon
  TempCHB,    //14  FloatB
  TempFLB,    //15  FloatA Modicon
  TempFHB,    //16  FloatB
  TOTAL_ERRORS,//17
  // leave this one
  TOTAL_REGS_SIZE//18
  // total number of registers for function 3 and 16 share the same register array
};
unsigned int holdingRegs[TOTAL_REGS_SIZE]; // function 3 and 16 register array
int BAUD_RATE = 9600;
int SLAVE_ADDRESS;
int RS485_CTRL_PIN = 0; //0 shuts down enable for RS485. If using put the pin of the control line here

int v_3;
int m_3;
int v_4;
int m_4;

void setup()
{
  /* parameters(long baudrate,
                unsigned char ID,
                unsigned char transmit enable pin,
                unsigned int holding registers size)

     The transmit enable pin is used in half duplex communication to activate a MAX485 or similar
     to deactivate this mode use any value < 2 because 0 & 1 is reserved for Rx & Tx
  */
  SLAVE_ADDRESS = (EEPROM.read(0));
  sensors.begin();
  //modbus_configure(9600, 1, 0, TOTAL_REGS_SIZE);
  modbus_configure(BAUD_RATE, SLAVE_ADDRESS, RS485_CTRL_PIN, TOTAL_REGS_SIZE);
  //BAUD_RATE must be 9600 or less for ATtiny
  //SLAVE_ADDRESS is 1-254
  //RS485_CTRL_PIN must be 2 or higher for RE/DE control. if 0 or 1 it will not enable RE/DE control
  //TOTAL_REGS_SIZE
}

void loop() {
  // modbus_update() is the only method used in loop(). It returns the total error
  // count since the slave started. You don't have to use it but it's useful
  // for fault finding by the modbus master.

  delayMicroseconds(5000); 
  sensors.requestTemperatures(); // Send the command to get temperatures
  float Temp = sensors.getTempCByIndex(0); // read DS18B20 sensor
  int tempc = (Temp * 100) ; // as a integer has no decimal. multiply the float by 100 give the ability to do
  // two place decimal places when polled and divided by 100 at the host system
  int tempf = (((Temp * 1.8) + 32) * 100); // as a integer has no decimal. multiply the float by 100 give the ability to do
  // two place decimal places when polled and divided by 100 at the host system
  float TempF=((Temp * 1.8) + 32);
  //****Read analog and do scaling using mx+y formula
  m_3 = analogRead(1);
  m_3 = analogRead(1);
  v_3 = ((m_3 * EEPROM.read(1)) + EEPROM.read(2));
  m_4 = analogRead(2);
  m_4 = analogRead(2);
  v_4 = ((m_4 * EEPROM.read(3)) + EEPROM.read(4));
  //****
  //*** holding Registers for polling
  holdingRegs[0] = v_3;  //Value3
  holdingRegs[1] = v_4;  //value4
  holdingRegs[2] = tempc;
  holdingRegs[3] = tempf;
  holdingRegs[4] = m_3;
  holdingRegs[5] = EEPROM.read(1);
  holdingRegs[6] = EEPROM.read(2);
  holdingRegs[7] = m_4;
  holdingRegs[8] = EEPROM.read(3);
  holdingRegs[9] = EEPROM.read(4);
  holdingRegs[10] = EEPROM.read(0);
  holdingRegs[11] = BAUD_RATE;
  //***************
  float float_number= Temp;
  //unsigned int holdingRegs[13] = f_2uint_int1(float_number);           // split the float into 2 unsigned integers
  //unsigned int holdingRegs[14] = f_2uint_int2(float_number);
  unsigned int reg0 = f_2uint_int1(float_number);           // split the float into 2 unsigned integers
  unsigned int reg1 = f_2uint_int2(float_number);
  holdingRegs[13] = reg0;
  holdingRegs[14] = reg1;
  //*****************************
  float_number= TempF;
  //unsigned int holdingRegs[15] = f_2uint_int1(float_number);           // split the float into 2 unsigned integers
  //unsigned int holdingRegs[16] = f_2uint_int2(float_number);
  unsigned int reg2 = f_2uint_int1(float_number);           // split the float into 2 unsigned integers
  unsigned int reg3 = f_2uint_int2(float_number);
  holdingRegs[15] = reg2;
  holdingRegs[16] = reg3;
  //******************
  modbus_update(holdingRegs); // wait for modbus poll through serial port
 //**** Update eeprom with any new values
  EEPROM.update(0, holdingRegs[10]);
  EEPROM.update(1, holdingRegs[5]);
  EEPROM.update(2, holdingRegs[6]);
  EEPROM.update(3, holdingRegs[8]);
  EEPROM.update(4, holdingRegs[9]);
}
unsigned int f_2uint_int1(float float_number) {             // split the float and return first unsigned integer
  union f_2uint {
    float f;
    uint16_t i[2];
  };
  union f_2uint f_number;
  f_number.f = float_number;
  return f_number.i[0];
}
unsigned int f_2uint_int2(float float_number) {            // split the float and return first unsigned integer
  union f_2uint {
    float f;
    uint16_t i[2];
  };
  union f_2uint f_number;
  f_number.f = float_number;
  return f_number.i[1];
}
