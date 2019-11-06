/* nRF24L01_BLE_Recv_Send_v.1_20170519 */
#include <RF24.h>    // Подключаем библиотеку, для работы с nRF24L01 (SPI in Arduino Uno/Nano: MOSI pin 11, MISO pin 12, SCK pin 13)
#include <BTLE.h>                                 // Подключаем библиотеку "BTLE", для превращения (nRF24L01-радио) в (nRF24L01-BLE)
RF24 radio(11,12);                 // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01 (CE, CSN)
BTLE btle(&radio);                                                            // Создаём объект btle для работы с библиотекой "BTLE"
char BLEname[17];                                                     // массив для (имя-BLE) - максимальная длина (16)! символов
#define PASSWORD   "Samsung Galaxy S7"   // Замените! Dmitry OSIPOV на имя вашего устройства. Пароль - максимальная длина (14)! символов
bool evenNumber  = false;                                                                                   // для проверки четности
bool lightsOn = false;
void setup() 
{
  radio.begin();
  //radio.setAutoAck(false);
  Serial.begin(9600);
  pinMode(27, OUTPUT);
  
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
}                                                                      // Serial.begin(9600);  // Serial.println(str);
void loop() {
  String str = "";                                                    // для сбора в оду строку значений, от разных типов переменных
  if (evenNumber % 2 == millis() / 200 % 2) {//тактирование(проверкой четности и остатком от деления youtube.com/watch?v=yVxjzu__ukI
    str = (String) analogRead(A1) + "_hedbun_" + analogRead(A2);                        // собираем разные типы данных, в строку
    str.toCharArray(BLEname, sizeof(BLEname));                        // заполняем, массив (имя-BLE), из винегрета данных, через str
    btle.begin(BLEname);                                                                                        // Включаем передачу
    btle.advertise(0, 0);                  // разрешаем Трансляцию (рекламного пакета) с дополнительной полезной нагрузкой "BLEname"
    evenNumber = !evenNumber;                                                                                // инвертируем четность
  }
  else if (btle.listen()) {                                                             // Если в буфере BLE имеются принятые данные  
    Serial.print("Got payload: ");
    
//    for (uint8_t i = 2; i < (btle.buffer.pl_size) - 6; i++)
//    {
//      //str += (char (btle.buffer.payload[i])); // собираем в str принятые данные
//      Serial.print(btle.buffer.payload[i]); Serial.print(" ");
//    }
    
    for (uint8_t i = 2; i < (btle.buffer.pl_size) - 6; i++) 
    { 
      Serial.print(btle.buffer.payload[i],HEX); Serial.print(" ");
    }
    
    Serial.println("done.");
    
    
//    if (str.startsWith(PASSWORD)) {                                // (защита от спама) только если str начинается с "Dmitry OSIPOV"      
//      if (!lightsOn) { digitalWrite(27, HIGH); lightsOn = true; }
//      else { digitalWrite(27, LOW); lightsOn = false; }
//      //str.replace(PASSWORD, "");                                                             // (удаляем / заменяем) из str PASSWORD
//      //pinMode((str.charAt(2)), OUTPUT);                          // Serial.println(str.charAt(2),HEX);  // делаем нужный PIN, OUTPUT
//      //digitalWrite((str.charAt(2)), (str.charAt(3)));            // Serial.println(str.charAt(3),HEX);     // управляем нужными PINs
//    }
  }
}
