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
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

int sensorPin = 2; //the analog pin the TMP36's Vout (sense) pin is connected to
int MsensorPin = A0;  
int sensorValue = 0;  
int percent = 0;

Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);


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

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  
  Serial.print("Temperature = "); Serial.print(bme.temperature); Serial.println(" *C");
  Serial.print("Humidity = "); Serial.print(bme.humidity); Serial.println(" %");
  Serial.println();
  
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  Serial.print(temperatureC); Serial.println(" degrees C");
    
  sensorValue = analogRead(MsensorPin);
  percent = convertToPercent(sensorValue);
  
  Serial.print("\nPercent: ");
  Serial.print(percent);
  Serial.println("%");

  ////////////Transission
  String finalText;
  finalText=  "Tem= " + String(bme.temperature)+" / "+ temperatureC + " *C " + "Hum= " + String(bme.humidity) + " %" + percent + "moistSen%" ;  
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
