//include relevant libraries
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
const int DRY = 700;
const int WET = 120;
int moist = 0;

//variables to store the temperature values.
float temp;
float humid;

String result = "";

//create the dht temperature and humidity 
dht DHT;

void setup() {
  //beign serial communication
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  //define chk to be the dht11 read function
  int chk = DHT.read11(DHT11PIN);
  
  //read the rain value and set it to the moist variable
  moist = analogRead(A0);
  //map the moist value between 0 and 100, to get a percentage map(value, fromlow, fromhigh, tolow, tohigh)
  moist  = map(moist, WET, DRY, 100, 0);

  //code for sending the weather data to the machine learning algorithm 
  const int32_t length = 3;
  //set values to be the rain value, temperature, and humidity
  float values[length] = {moist, temp, humid};
  //the machine learning algorithm then outputs the predicted class (wet weather, wet oval, or no rain) based on the variables.
  const int32_t predicted_class = RAIN_predict(values, length);

  //change the integer values to be changed into strings
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
  Serial.println(moist);
  Serial.println(DHT.temperature);
  Serial.println(DHT.humidity);
  delay(2200);
}
  
