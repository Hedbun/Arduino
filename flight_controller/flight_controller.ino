/*
 Name:		flight_controller.ino
 Created:	11/12/2019 01:14:27
 Author:	HEDBUN
*/

#include <RF24.h>
#include <BTLE.h> // converts from (nRF24L01-radio) to (nRF24L01-BLE)
#include <Servo.h>
Servo ESC1; // library for working with ESC(electronic speed controller) that controls brushless DC motor
RF24 radio(11, 12); // (CE, CSN) pins
BTLE btle(&radio);
constexpr auto PASSWORD1 = "S7";
constexpr auto PASSWORD2 = "J5";
int motorSpeedInt = 0; // by default, 0 speed is provided to ESC in the main loop

// the setup function runs once when you press reset or power the board
void setup() 
{
	//ESC1.attach(27, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)   
	Serial.begin(9600);

	// providing power to the nRF24L01 unit, that has VCC pin connected to 25 pin on Arduino Due
	pinMode(25, OUTPUT);
	digitalWrite(25, HIGH);

	// initializing nRF24L01 unit
	btle.begin("");
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	if (btle.listen())
	{
		String motorSpeed = ""; // a value received from external BLE device(mobile phone) that will be supplied to ESC. Represents speed of the motor.
		String deviceName = ""; // name of the BLE device.
		int bytesToSkip = 0;
		bytesToSkip = btle.buffer.payload[0]; // first byte of the packet tells how many bytes till the end of the device name
		//Serial.println(bytesToSkip);
		// reading device name
		for (uint8_t i = 2; i < bytesToSkip + 1; i++)
		{
			deviceName += char(btle.buffer.payload[i]);
		}

		if (deviceName == PASSWORD1 || deviceName == PASSWORD2) // for now we use device name to ignore data from other devices that are non pre-defined in this code. Will need to find better solution later.
		{
			bytesToSkip = bytesToSkip + 1 + 4; // we know that advertisment packet contains device name + manufacturer data, so here we are skipping right to the start of the motor speed value
			// reading motor speed
			for (uint8_t i = bytesToSkip; i < (btle.buffer.pl_size) - 6; i++) // motor speed is the last piece of data in the packet, so reading till the end
			{
				motorSpeed += char(btle.buffer.payload[i]);
			}
			motorSpeedInt = motorSpeed.toInt();
			Serial.print(deviceName); Serial.print(": "); Serial.println(motorSpeedInt);
		}
	}
	//ESC1.write(motorSpeedInt);    // Send the signal to the ESC 
  
}
