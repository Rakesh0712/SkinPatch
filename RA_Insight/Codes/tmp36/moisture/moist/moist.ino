
int sensor=A0; //initializing the analog pin

 
int sensorValue = 0;  
int percent = 0;


int value;

void setup(){

Serial.begin(9600);

pinMode(sensor, INPUT); // assing the pin mode type of the pin as input or output
}

void loop(){

  //value=analogRead(sensor); //reading the valus from the sensor

  sensorValue = analogRead(sensor);
  percent = convertToPercent(sensorValue);
  printValuesToSerial();
 // delay(1000);
  delay(2000);
  
}
