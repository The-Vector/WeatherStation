#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SERIAL_BAUD 115200 //make sure this is the same in arduino.ino

ESP8266WebServer server(80);

String receivedCommand = "";
bool dataIn = false;

const String html_page = "<!DOCTYPE html>"
                         "<html>"
                         "  <head>"
                         "    <meta name='viewport' content='width=device-width, initial-scale=1.0' />"
                         "  </head>"
                         "  <body>"
                         "    <p>data from Arduino:</p>"
                         "    <pre id='reading'></pre>"
                         "    <script>"
                         "      (function() {"
                         "        /* get new data every second*/"
                         "        setInterval(function() {"
                         "          fetch('/reading')"
                         "          .then(response => { return response.text();})"
                         "          .then(text => {"
                         "            document.getElementById('reading').textContent = text;"
                         "          });"
                         "        }, 100);"
                         "      })();"
                         "    </script>"
                         "  </body>"
                         "</html>";

const IPAddress serverIPAddress(10, 0, 0, 7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  // put your main code here, to run repeatedly:

  server.on("/reading", []() { //send raw serial data
    //ahh put stuff below into here ??
  });
  
  while (Serial.available())
  {
    char c = Serial.read(); //read it

    if (c == '[')
    {
      //this is the start of the command string
      receivedCommand = "";
      dataIn = true;
    }
    //otherwise, we are still reading the command string:
    else if (dataIn && c != ']')
    {
      receivedCommand += c;
    }

    else if (dataIn && c == ']')
    {
      //finished receiving the command, process it
      Serial.print("XC4411 has been running for ");
      Serial.print(millis(), DEC);
      Serial.println(" milliseconds..");
       Serial.print("Received command was '");
      Serial.print(receivedCommand);
      Serial.print("' - action: ");

      if (receivedCommand == "LEDON")
      {
        Serial.println("TURN LED ON");
        digitalWrite(led_pin, HIGH);
      }
      else if (receivedCommand == "LEDOFF")
      {
        Serial.println("TURN LED OFF");
        digitalWrite(led_pin, LOW);
      }
      else if (receivedCommand == "LEDTOGGLE")
      {
        Serial.println("CHANGE LED");
        digitalWrite(led_pin, !digitalRead(led_pin));
      }
      Serial.println("---------------------------------------");
    }
  }
  delay(10);
}
