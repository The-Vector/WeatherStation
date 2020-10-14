//iunclude relevant libraries
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

//define input pins and serial to output data to web server
#define DHT11PIN 7
#define moistPin A0
#define SERIAL_BAUD 115200

//define what analog values are wet and dry
const int DRY = 750;
const int WET = 150;
int moist = 0;

float temp;
float humid;

String result = "";

//create the dht temperature and humidity 
dht DHT;

void setup() {
  //beign serial communication
  Serial.begin(SERIAL_BAUD);
  //set the rain voltage pin to output, and low
  pinMode(moistPin, OUTPUT);
  digitalWrite(moistPin, LOW);
}

void loop() {
  //define chk to be the dht11 read function
  int chk = DHT.read11(DHT11PIN);
  
  //set the rain sensor voltage to high to allow the data to be read 
  digitalWrite(moistPin, HIGH);
  //read the rain value and set it to the moist variable
  moist = analogRead(0);
  digitalWrite(moistPin, LOW);
  //map the moist valeu between 0 and 60.4, to allow the machine learning algorithm to correctly use the data
  moist = map(moist, WET, DRY, 60.4, 0);

  //set the moist vale to be readable by the machine learning algorithm. 
  moist = moist * 0.05;

  //machine learning stuff
  const int32_t length = 3;
  //set values to be the rain value, temperature, and humidity
  float values[length] = {moist, temp, humid};
  //the machine learning algorithm then outputs the predicted class (wet weather, wet oval, or no rain) based on the variables.
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
  //print the results to serial so the web server can read and display them. 
  Serial.println("[" + result); //"[" allows the web server to start recording data
  Serial.println(DHT.temperature);
  Serial.println(DHT.humidity);
  delay(1000);
}
  
