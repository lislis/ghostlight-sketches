/*
  ESP32 flashlight v2

  - Connects to websocket
  - Sends out id as registration
  - Receives rumble and light signals to forward to components 

  Assuming the
  - motor is connected (via a transistor) to D19
  - cob LED is connected to D18

  and a physical switch can turn the board on and off

*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>


const char* ssid = "ssid";
const char* password = "password";
String id = "light01";
const char* websockets_server_host = "localhost"; //Enter server adress
const uint16_t websockets_server_port = 3000; // Enter server port

using namespace websockets;

const int MOTOR_PIN = 19; // 35; //D18;
const int LED_PIN = 4; //38; //D19;

bool rumble = false;
bool light = false;

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
      String prefix = "this-is-light___";
      client.send(prefix + id);
  } else {
      Serial.println("Not Connected!");
  }
  
  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message){
      Serial.print("Got Message: ");
      if (message.data() == "rumble-true") {
        rumble = true;
      }
      if (message.data() == "rumble-false") {
        rumble = false;
      }
      if (message.data() == "light-true") {
        light = true;
      }
      if (message.data() == "light-false") {
        light = false;
      }
      Serial.println(message.data());
  });

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // let the websockets client check for incoming messages
  if(client.available()) {
      client.poll();
  }

  if (light == true) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  if (rumble == true) {
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
  }
}
