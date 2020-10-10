#include <dht.h>
#include "RAIN.h" //machine learning was helped by emlearn,referenced below:
/*misc{emlearn,
  author       = {Jon Nordby},
  title        = {{emlearn: Machine Learning inference engine for 
                   Microcontrollers and Embedded Devices}},
  month        = mar,
  year         = 2019,
  doi          = {10.5281/zenodo.2589394},
  url          = {https://doi.org/10.5281/zenodo.2589394}
}*/

#define DHT11PIN A1
#define moistPin A0
#define SERIAL_BAUD 115200

const int DRY = 750;
const int WET = 150;
int moist = 0;

float temp;
float humid;

String result = "";

dht DHT;

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(moistPin, OUTPUT);
  digitalWrite(moistPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  DHT.read11(DHT11PIN);
  humid = DHT.humidity;
  temp = DHT.temperature;
  
  digitalWrite(moistPin, HIGH);
  delay(3000);
  moist = analogRead(0);
  digitalWrite(moistPin, LOW);
  moist = map(moist, WET, DRY, 60.4, 0);

  switch(moist){
    case 0 ... 10:
      moist = 0;
      break;
    case 11 ... 16:
      moist = 0.5;
      break;
    default:
      break;
  }
  
  const int32_t length = 3;
  float values[length] = {moist, temp, humid};
  const int32_t predicted_class = RAIN_predict(values, length);

  switch(predicted_class){
    case(0):
      result = "no rain";
      break;
    case(1):
      result = "wet oval";
      break;
    case(2):
      result = "rain";
      break;
  }
  //Serial.println(result);
  result = "[" + result + "]";
  Serial.println(result); //send serial data to wifi module
  result = "[" + String(temp) + "]";
  Serial.println(result);
  result = "[" + String(humid) + "]";
  Serial.println(result);
}
  
