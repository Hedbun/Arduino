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
int sliderValueInt = 0;

void setup() 
{
  //ESC1.attach(27, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)   
  Serial.begin(9600);
  
  pinMode(25, OUTPUT); //nrf VSC
  digitalWrite(25, HIGH); //nrf VSC
  
  btle.begin("");
}          
                                                           
void loop() 
{
  if (btle.listen()) 
  {
    String sliderValue = "";
    String deviceName = "";
    int bytesToSkip = 0;
    bytesToSkip = btle.buffer.payload[0];
    //Serial.println(bytesToSkip);
    for (uint8_t i = 2; i < bytesToSkip + 1; i++)
    {
      deviceName += char(btle.buffer.payload[i]);
    }    

    if (deviceName == PASSWORD1 or deviceName == PASSWORD2)
    {
      bytesToSkip = bytesToSkip + 1 + 4;
      for (uint8_t i = bytesToSkip; i < (btle.buffer.pl_size) - 6; i++)
      {
        sliderValue += char(btle.buffer.payload[i]);
      }
      sliderValueInt = sliderValue.toInt();
      Serial.print(deviceName); Serial.print(": "); Serial.println(sliderValueInt);
    }        
  }
  //ESC1.write(sliderValueInt);    // Send the signal to the ESC 
}
