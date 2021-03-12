/*
* Arduino wireless Temperature & Humidity transmission using RF transreceiver
*     Testing code - Transmitter Code BME 680+ tmp36+ nRFL+ arduino nano Basic version
*                
* by Rakesh Dhakshina Murthy (Rocky D.Luffy)
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

/*
 * Connection Instruction - RF receiver -SPI BME- I2C 
 */
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <SPI.h>



int sensorPin = 2; //the analog pin the TMP36's Vout (sense) pin is connected to
int MsensorPin = A0;  
int sensorValue = 0;  
int percent = 0;
int tempOffset = +3;  //approximately -50 for 3.3v base and 2 for 5V base
int humidityOffset =0; //approximately +50 for 3.3v base and 3 for 5V base


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001"; //receiver address

void setup() {

   Serial.begin(9600);
  while (!Serial);
  Serial.println(F("Temperature/Humidity Test....!!!"));
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop() {


  

  
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
  float temperatureC = ((voltage - 0.5) * 100)+ tempOffset ;  //converting from 10 mv per degree wit 500 mV offset
  
  
    
  sensorValue = analogRead(MsensorPin);
  percent = convertToPercent(sensorValue);


  Serial.print(temperatureC); Serial.print(" degrees C");
  Serial.print("\nPercent: ");
  Serial.print(percent);
  Serial.println("%");

  
  String finalText;
  finalText=  "Tem= " +  String(temperatureC) + " *C " + "Hum= " + String(percent)+ "%" ;  
  Serial.print(finalText);
  Serial.println();
  
  const char text[30];
  finalText.toCharArray(text, 30);
  radio.write(&text, sizeof(text));



  
  delay(3000);
}


int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 465, 0, 100);
  return percentValue;
}
