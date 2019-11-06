/* nRF24L01_BLE_Recv_Send_v.1_20170519 */
#include <RF24.h>    // Подключаем библиотеку, для работы с nRF24L01 (SPI in Arduino Uno/Nano: MOSI pin 11, MISO pin 12, SCK pin 13)
#include <BTLE.h>                                 // Подключаем библиотеку "BTLE", для превращения (nRF24L01-радио) в (nRF24L01-BLE)
#include <Servo.h>
Servo ESC1;
RF24 radio(11,12);                 // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01 (CE, CSN)
BTLE btle(&radio);                                                            // Создаём объект btle для работы с библиотекой "BTLE"
char BLEname[17];                                                     // массив для (имя-BLE) - максимальная длина (16)! символов
#define PASSWORD1   "S7"   // Замените! Dmitry OSIPOV на имя вашего устройства. Пароль - максимальная длина (14)! символов
#define PASSWORD2   "J5"
int value = 0;

void setup() 
{
  ESC1.attach(27, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds) 
  radio.begin();
  Serial.begin(9600);
  
  pinMode(25, OUTPUT); //nrf VSC
  digitalWrite(25, HIGH); //nrf VSC
  String str = "";
  str = (String) "_hedbun_";                        // собираем разные типы данных, в строку
  str.toCharArray(BLEname, sizeof(BLEname));        
  btle.begin(BLEname);
}          
                                                           
void loop() 
{
  if (btle.listen()) 
  {                                        
    String strData;
    for (uint8_t i = 2; i < (btle.buffer.pl_size) - 6; i++) 
    { 
      strData+= (char(btle.buffer.payload[i]));       
    }       

    if(strData.startsWith(PASSWORD1) or strData.startsWith(PASSWORD2))
    {   
      String valueStr;   
      for (uint8_t i = 8; i < (btle.buffer.pl_size) - 6; i++) 
      { 
        valueStr += char(btle.buffer.payload[i]); // (value between 0 and 180)
      } 
      //Serial.print("Str: ");Serial.println(valueStr);
      value = valueStr.toInt();
    }
  }
  Serial.print("Int: ");Serial.println(value);
  ESC1.write(value);    // Send the signal to the ESC
}
