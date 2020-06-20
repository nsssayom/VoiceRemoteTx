#include <RF24.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24Audio.h"
#include "printf.h"                         // General includes for radio and audio lib

#define STX                 0x02            //START OF TEXT
#define ETX                 0x03            //END OF TEXT
#define EOT                 0x04            //END OF TRANSMISSION

RF24 radio(7, 8);                           // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio, 1);                // Set up the audio using the radio, and set to radio number 0

/***** Voice IOs *****/
const byte btnVoiceToggle = 3;
const byte ledVoiceToggle = 4;
volatile byte voiceState = LOW;
volatile byte buttonCleared = HIGH;

const byte btnX = A1;
const byte btnY = A2;
const byte btnA = A3;
const byte btnB = A4;


void setup() {
  Serial.begin(115200);         // Enable Arduino serial library

  printf_begin();               // Radio library uses printf to output debug info
  radio.begin();                // Must start the radio here, only if we want to print debug info
  rfAudio.begin();              // Start up the radio and audio libararies
  
  pinMode(btnVoiceToggle, INPUT);
  
  pinMode(btnX, INPUT_PULLUP);
  pinMode(btnY, INPUT_PULLUP);
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  
  pinMode(ledVoiceToggle, OUTPUT);
  digitalWrite(ledVoiceToggle, LOW);
  attachInterrupt(digitalPinToInterrupt(btnVoiceToggle), transmitAudio, CHANGE);
}

void transmitAudio(){
  voiceState = !voiceState;
    if (voiceState){
      Serial.println("Transmission Starting...");
      digitalWrite(ledVoiceToggle, HIGH);
      rfAudio.transmit();
      
      const String buttonString = "...............................";
      char buttonStateBuff[32];
      buttonString.toCharArray(buttonStateBuff, 32);
      buttonStateBuff[0] = STX;
      //buttonStateBuff[30] = EOT;
      radio.write(buttonStateBuff, 32);
      delay(20);
      
      rfAudio.broadcast(1);
      return;
  }
  else{
    Serial.println("Transmission Terminating...");
    rfAudio.receive();
    delay(20);
    
    const String buttonString = "...............................";
    char buttonStateBuff[32];
    buttonString.toCharArray(buttonStateBuff, 32);
    //buttonStateBuff[0] = STX;
    buttonStateBuff[30] = EOT;
    
    rfAudio.transmit();
    radio.write(buttonStateBuff, 32);
    
    digitalWrite(ledVoiceToggle, LOW);
    rfAudio.receive();
    return;
  }
}

void transmitButtonReading(String buttonState){
     if (buttonState == "0000"){
        if (buttonCleared){
            return;
        }
        else{
            buttonCleared = HIGH;
        }
     }
     else{
        buttonCleared = LOW;
     }
     
     rfAudio.transmit();
     const String buttonString = ".<ButtonActn>" + buttonState + "</ButtonActn>.";
     char buttonStateBuff[32];
     buttonString.toCharArray(buttonStateBuff, 32);
     buttonStateBuff[0] = STX;
     buttonStateBuff[30] = EOT;
     radio.write(buttonStateBuff, 32);
     rfAudio.receive();
}


void HandleButtons(){
  boolean buttonXState = !digitalRead(btnX);
  boolean buttonYState = !digitalRead(btnY);
  boolean buttonAState = !digitalRead(btnA);
  boolean buttonBState = !digitalRead(btnB);

  char buttonState[] = {'0','0','0','0'};
  
  if (buttonXState){
    buttonState[0] = '1';
  }
  else{
    buttonState[0] = '0';
  }

  if (buttonYState){
    buttonState[1] = '1';
  }
  else{
    buttonState[1] = '0';
  }

  if (buttonAState){
    buttonState[2] = '1';
  }
  else{
    buttonState[2] = '0';
  }
  
  if (buttonBState){
    buttonState[3] = '1';
  }
  else{
    buttonState[3] = '0';
  }
  String stateString(buttonState);
  transmitButtonReading(stateString);
}

void loop() {
  HandleButtons();
  delay(50);
}
