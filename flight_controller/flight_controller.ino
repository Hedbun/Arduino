/*
 Name:		flight_controller.ino
 Created:	11/12/2019 01:14:27
 Author:	HEDBUN
*/

#include <RF24.h>
#include <BTLE.h> // converts from (nRF24L01-radio) to (nRF24L01-BLE)
#include <Servo.h> // library for working with ESC(electronic speed controller) that controls brushless DC motor

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
RF24 radio(55, 56); // (CE, CSN) pins
BTLE btle(&radio);
constexpr auto PASSWORD1 = "S7";
constexpr auto PASSWORD2 = "J5";
int motorSpeedInt = 0; // by default, 0 speed is provided to ESC in the main loop
int motorStopTimer = 0; // timer time for emergency motor stop

// the setup function runs once when you press reset or power the board
void setup() 
{
	ESC1.attach(29, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)   
	ESC2.attach(28, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)   
	ESC3.attach(26, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)   
	ESC4.attach(27, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)   		
	Serial.begin(9600);

	// providing power to the nRF24L01 unit, that has VCC pin connected to 57 pin on Arduino Due
	pinMode(57, OUTPUT);
	digitalWrite(57, HIGH);

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
			motorStopTimer = 0; // reset timer
			Serial.print(deviceName); Serial.print(": "); Serial.println(motorSpeedInt);
		}
	}

	motorStopTimer++;
	if (motorStopTimer >= 100)
	{
		motorSpeedInt = 0;
		motorStopTimer = 0;
	}
	
	ESC1.write(motorSpeedInt);    // Send the signal to the ESC 
	ESC2.write(motorSpeedInt);    // Send the signal to the ESC 
	ESC3.write(motorSpeedInt);    // Send the signal to the ESC 
	ESC4.write(motorSpeedInt);    // Send the signal to the ESC 
  
	/*Serial.print(millis());
	Serial.print(" - ");
	Serial.print(motorStopTimer);
	Serial.print(" - ");
	Serial.println(motorSpeedInt);*/
}
