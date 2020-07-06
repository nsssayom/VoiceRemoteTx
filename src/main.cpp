/*
 Written by Sayom, Nazmus Shakib <nsssayom@gmail.com>
 for Rise Up Labs

 This program is firmware for a wireless remote control System
 based on 2.4 GHZ Radio. Along with key press this system is
 capable of transmitting audio command.

 */

#include <RF24.h>
#include <SPI.h>	   // SPI Library for nRF24L01
#include "nRF24L01.h"  // Radio hardware library
#include "RF24Audio.h" // Audio library
#include "printf.h"	   // General includes for radio and audio lib
#include "Keypad.h"	   // Keypad library for 3x4 analog matrix keypad
/*
	Defining ASCII characters to be used in headers 
*/

#define STX 0x02 //START OF TEXT
#define ETX 0x03 //END OF TEXT
#define EOT 0x04 //END OF TRANSMISSION

/*
	Initializing radio and audio hardwares
*/

RF24 radio(7, 8);			 // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio, 1); // Set up the audio using the radio, and set to radio number 0

/*
	Initializing keypad 
*/

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};
byte rowPins[rows] = {9, A1, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {2, 3, 4};		//connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

/*
	Flag variable to indicate audio transmission 
*/

volatile byte voiceState = LOW;

/*
	Function for commensing audio transmission 
*/

void StartAudioStransmission()
{
	//delay(20);
	rfAudio.transmit();

	const String buttonString = "...............................";
	char buttonStateBuff[32];
	buttonString.toCharArray(buttonStateBuff, 32);
	buttonStateBuff[0] = STX;
	
	if(radio.write(buttonStateBuff, 32)){
		Serial.println("Transmission Started");
	}
	else
	{
		Serial.println("Transmission Beginning Failed");
	}
	//delay(20);
	rfAudio.broadcast(1);
	return;
}

/*
	Function to stop audio transmission 
*/

void EndAudioTransmission()
{
	delay(20);
	rfAudio.receive();
	/*
		Audio transmission header is EOT..............................
		Audio signal will be followed by this character sequence.
	*/
	delay(20);
	const String buttonString = "...............................";
	char buttonStateBuff[32];
	buttonString.toCharArray(buttonStateBuff, 32);
	buttonStateBuff[30] = EOT;

	// Commencing header transmission
	rfAudio.transmit();
	if (radio.write(buttonStateBuff, 32)){
		Serial.println("Transmission Terminated");
	}
	else{
		Serial.println("Transmission Termination failed");
	}
	//delay(10);
	// Closing audio transmission
	rfAudio.receive();
	return;
}

/*
	Data format is STX<inpt>0000000000000000</inpt>EOT
	1 or 0 value of every numerical bit represents pressed of not-pressed state
	of each button. The first numerical bit represents key '0', Second on 
	represents key '1' and so on. 11th and 13th to 16th bit is not used. 
	
*/

void transmitButtonSignal(String buttonState)
{	
	const String buttonString = ".<inpt>" + buttonState + "</inpt>.";
	
	char buttonStateBuff[32];
	buttonString.toCharArray(buttonStateBuff, 32);
	buttonStateBuff[0] = STX;
	buttonStateBuff[30] = EOT;

	// Switching to transmission mode
	rfAudio.transmit();
	// Commensing button press signal transmission

	if (radio.write(buttonStateBuff, 32)){
		Serial.println("Signal Sending Success: " + String(buttonStateBuff) + "Size: " + sizeof(buttonStateBuff));
	}
	else{
		Serial.println("Signal Sending Failed: " + String(buttonStateBuff) + "Size: " + sizeof(buttonStateBuff));
	}
	
	// Closing transmission mode
	rfAudio.receive();
}

/*
	Button handler function
*/

void HandleButtons(KeypadEvent key)
{
	// Button state array
	char buttonState[] = "0000000000000000";

	switch (keypad.getState())
	{
	case PRESSED:
	{
		switch (key)
		{
		case '0':
			buttonState[0] = '1';
			break;
		case '1':
			buttonState[1] = '1';
			break;
		case '2':
			buttonState[2] = '1';
			break;
		case '3':
			buttonState[3] = '1';
			break;
		case '4':
			buttonState[4] = '1';
			break;
		case '5':
			buttonState[5] = '1';
			break;
		case '6':
			buttonState[6] = '1';
			break;
		case '7':
			buttonState[7] = '1';
			break;
		case '8':
			buttonState[8] = '1';
			break;
		case '9':
			buttonState[9] = '1';
			break;
		case '*':
			return;
			//buttonState[10] = '1';
			//break;
		case '#':
			buttonState[11] = '1';
			break;
		}
		String stateString(buttonState);
		transmitButtonSignal(stateString);
		break;
	}
	case RELEASED:
		if (key == '*')
		{
			Serial.println("Event Log: Audio Button Released");
			EndAudioTransmission();
			break;
		}
	case HOLD:
		if (key == '*')
		{
			Serial.println("Event Log: Audio Button Held");
			StartAudioStransmission();
			break;
		}
	}
}

/*
	Bootstraping all necessary components
*/

void setup()
{
	Serial.begin(115200); // Enable Arduino serial library

	printf_begin();							// Radio library uses printf to output debug info
	radio.begin();							// Must start the radio here, only if we want to print debug info
	rfAudio.begin();						// Start up the radio and audio libraries
	keypad.setDebounceTime(10);
	keypad.addEventListener(HandleButtons); // Add an event listener for this keypad
	radio.printDetails();
}

void loop()
{
	char key = keypad.getKey();
	if (key) Serial.println(key);
}