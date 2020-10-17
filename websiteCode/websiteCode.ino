//this script has to be compiled with the "generic ESP8266 Module" board selected 

//including all the relevant arduino libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>

//defining the serial rate to "talk" and recieve data from the arduino
#define SERIAL_BAUD 115200

//set the name and password of the wifi network to allow people to connect
const char* ssid = "Weather Station";
const char* password = "12345678";

//tell the arduino that there will be a server at port 80
WiFiServer server(80);

void setup() {
  //begin the serial to communicate with the arduino
  Serial.begin(SERIAL_BAUD);
  //set the wifi mode and the soft access point
  WiFi.mode(WIFI_AP);
  boolean r = WiFi.softAP(ssid, password);

  //if the soft access point started, begin the server
  if (r) {
    server.begin();
    Serial.println("");
    Serial.print("Please connect to the Access Point: ");
    Serial.println(ssid);
    Serial.printf("then open %s in a web browser\n", WiFi.softAPIP().toString().c_str());
    Serial.println("");
  } else {
    Serial.println("FAILED to create Access Point");
    while(1){}
  }
}

void loop() {
  //set the client to be server availability
  WiFiClient client = server.available();
  //if a user/client connects to the server
  if(client){
    Serial.println("\n A Client just connected to the server");
    //while the client is still connected to the site/wifi network
    while(client.connected()) {
      if(client.available()){
        String clientMessage = client.readStringUntil('\r');
        if(clientMessage.length() == 1 && clientMessage[0] =='\n') {
          //wait until the wifi sends weather data
          while(Serial.read() != '[')
            continue;
          //save the weather data into veriables 
          String result = Serial.readStringUntil('\n');
          String rain_percent = Serial.readStringUntil('\n');
          String temp = Serial.readStringUntil('\n');
          String humid = Serial.readStringUntil('\n');
          //create a html page displaying the weather data for the client
          client.println(constructHTMLpage(result, rain_percent, temp, humid));
          break;
        }
      }
    }
    delay(1000);
    client.stop();
    Serial.println("\n The server has disconnected the Client");
  }
}

//construct a html page based on the weather data btained by the arduino
String constructHTMLpage(String result, String rain_percent, String temp, String humid){

  //top half is setup, not much happening client side 
  String HTMLpage = String("HTTP/1.1 200 OK\r\n") +
                            "Content-Type: text/html\r\n" +
                            "Connection: close\r\n" +
                            "Refresh: 5\r\n" +
                            "\r\n" +
                            "<!DOCTYPE HTML>" +
                            "<html><body>\r\n" +
                            "<h2>Weather Data</h2>\r\n" +
                            "<table><tr><th>Weather Result</th><th>Rain %</th><th>Temperature</th><th>Humidity</th></tr>\r\n";

  //add the results ( result temperature, humidity) to the table displayed with html
  HTMLpage = HTMLpage + String("<tr><td>");
  HTMLpage = HTMLpage + String(result);
  HTMLpage = HTMLpage + String("</td><td>");
  HTMLpage = HTMLpage + String(rain_percent);
  HTMLpage = HTMLpage + String("</td><td>");
  HTMLpage = HTMLpage + String(temp);
  HTMLpage = HTMLpage + String("</td><td>");
  HTMLpage = HTMLpage + String(humid);
  HTMLpage = HTMLpage + String("</td></tr>");
  //refresh the page every 2.5 seconds to keep the weather data relevant / up to date
  HTMLpage = HTMLpage + String("<script>setTimeout(() => { window.location.reload(false);  }, 2500);</script>\r\n");
  HTMLpage = HTMLpage + String("</table></body></html>\r\n");
  //returns the htmlpage string to the client
  return HTMLpage;
}
