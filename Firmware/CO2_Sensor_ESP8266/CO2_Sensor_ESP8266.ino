/*
  Basic Arduino example for K-Series sensor
  Created by Jason Berger
  Co2meter.com

  Rewritten for WiFi-functionality of an ESP8266/
  ESPtoy http://rayshobby.net/esptoy/
  and connected to a Senseair S8 CO2-sensor
  by Markus Ulsass/ markus.ulsass@protonet.info
  Protonet.info
  Info and Building instructions on http://experimental-platform.tumblr.com/
*/

#include <ESP8266WiFi.h>

const char* ssid     = "yourSSIDHere";  // put your SSID here
const char* password = "yourPasswordHere"; // put your password here

const char* host = "myPlatform.local"; // IP or address of Experimental Platform

byte readCO2[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};  //Command packet to read Co2 (see app note)
byte response[] = {0, 0, 0, 0, 0, 0, 0}; //create an array to store the response

// multiplier for value. default is 1. set to 3 for K-30 3% and 10 for K-33 ICB
int valMultiplier = 1;

void setup()
{
  Serial.begin(9600);     //Opens the serial port with a baud of 9600
  Serial.swap();          // Serial on ESP8266 normally connected to GPIO1 (TX) and GPIO3 (RX), swap it
  // maps it to GPIO15 (TX) and GPIO13 (RX) which is Pin 8 and 7 on ESPToy

  WiFi.begin(ssid, password); // initialize WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// Function for requesting data from the CO2-sensor

void sendRequest(byte packet[])
{
  while (!Serial.available()) //keep sending request until we start to get a response
  {
    Serial.write(readCO2, 7);
    delay(50);
  }

  int timeout = 0; //set a timeoute counter
  while (Serial.available() < 7 ) //Wait to get a 7 byte response
  {
    timeout++;
    if (timeout > 10)   //if it takes to long there was probably an error
    {
      while (Serial.available()) //flush whatever we have
        Serial.read();

      break;                        //exit and try again
    }
    delay(50);
  }

  for (int i = 0; i < 7; i++)
  {
    response[i] = Serial.read();
  }
}

unsigned long getValue(byte packet[])
{
  int high = packet[3];                        //high byte for value is 4th byte in packet in the packet
  int low = packet[4];                         //low byte for value is 5th byte in the packet


  unsigned long val = high * 256 + low;              //Combine high byte and low byte with this formula to get value
  return val * valMultiplier;
}

void loop()
{

  sendRequest(readCO2);
  unsigned long valCO2 = getValue(response);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {

    return;
  }

  // We now create a URI for the request
  String url = "/co2/update?status=";
  url += valCO2;
  url += "&location=mySensorName";  // put your sensor name in here

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Delay between URI-requests
  // Do not keep the host too busy ;)
  delay(2000);

}
