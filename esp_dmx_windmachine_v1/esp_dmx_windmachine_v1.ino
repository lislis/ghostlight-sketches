/*
  ESP32 dmx windmachine v1

  - Connects to websocket
  - Sends out id as registration
  - receives strength value
  - forwards it to serial aka arduino uno

  RX and TX or esp and arduino need to be cross connected
  and share same ground

*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>


//const char* ssid = "ssid";
//const char* password = "password";
//const char* websockets_server_host = "localhost";
//const uint16_t websockets_server_port = 3000;

//const char* ssid = "FFT-Gast";
//const char* password = "2021socialplacetobe@FFT#";
const char* ssid = "ghostlike-iot";
const char* password = "tellittomyheart";
//const char* websockets_server_host = "172.16.60.180";
const char* websockets_server_host = "10.3.141.1";
const uint16_t websockets_server_port = 3000;

// change this for every board!!
String id = "dmxWindmachine";

using namespace websockets;

int strength = 0;
int channel_red = 1;
int channel_green = 2;
int channel_blue = 3;
int channel_wind = 4;
int channel_hallway = 5;

int red_strength = 0;
int green_strength = 0;
int blue_strength = 0;
int wind_strength = 0;
int hallway_strength = 0;

WebsocketsClient client;
void setup() {
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }

  // Check if connected to wifi
  if(WiFi.status() != WL_CONNECTED) {
      Serial.println("No Wifi!");
      return;
  }

  Serial.println("Connected to Wifi, Connecting to server.");
  // try to connect to Websockets server
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  if(connected) {
      Serial.println("Connected!");
      String prefix = "this-is-windmachine";
      client.send(prefix + id);
  } else {
      Serial.println("Not Connected!");
  }
  
  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message){
      //Serial.print("Got Message: ");
      Serial.println(message.data());
  });
}

void loop() {
  // let the websockets client check for incoming messages
  if(client.available()) {
      client.poll();
  }
}
