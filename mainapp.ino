#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/f25456a6-4b54-44df-985d-b4aad9806f92 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float hum;
  float temp;
  float soilmoister;
  bool pump;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(-1);


int dhtpin = D4;
int soilpin = A0;
int pumppin = D7;

#define DHTTYPE DHT11
DHT dht(dhtpin,DHTTYPE);


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  dht.begin();
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Defined in thingProperties.h
  initProperties();
  pinMode(soilpin,OUTPUT);
  pinMode(pumppin,OUTPUT);

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  
    temp =dht.readTemperature(); // Read temperature as Celsius
    hum =dht.readHumidity(); //Read Humidity
    float value = analogRead(soilpin);
    soilmoister = map(value,0,1024,100,0);
  
  Serial.print("humidity: ");
  Serial.print(hum);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C ");
  Serial.println("");
  Serial.print("soilmoister: ");
  Serial.println(soilmoister);
  
  displayparams();
  display.display();
  if(soilmoister<20){
    digitalWrite(pumppin,HIGH);
  }
  else{
    digitalWrite(pumppin,LOW);
  }

}


void displayparams(){

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("hum = ");
  display.print(hum);
  display.setCursor(0,10);
  display.print("temp = ");
  display.print(temp);
  display.setCursor(0,20);
  display.print("soil = ");
  display.print(soilmoister);
  delay(2000);
}





void onPumpChange() {
  // Do something
  if(pump == HIGH){
    digitalWrite(pumppin,HIGH);
  }
  else{
    digitalWrite(pumppin,LOW);
  }
}
