/*
  ESP32 sensor v1

  - Connects to websocket
  - Sends out id as registration
  - Reads resistance and sends it
  - Receives threshold info
  - Lights a row of LEDs in sensor color
  - Indicates threshold crossing through lighting up another row of LEDs
  
*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>


const char* ssid = "ssid";
const char* password = "password";
const char* websockets_server_host = "localhost";
const uint16_t websockets_server_port = 3000;

// change this for every board!!
String id = "sensorGREEN";

using namespace websockets;

const int RESISTANCE_PIN = 19;
const int LED_COLOR_PIN = 4; 
//const int LED_TRIGGER_1 = 
//const int LED_TRIGGER_2 =
// const int LED_TRIGGER_3 =  

int reading = 0;
int threshold = 0;

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
      String prefix = "this-is-sensor___";
      client.send(prefix + id);
  } else {
      Serial.println("Not Connected!");
  }
  
  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message){
      Serial.print("Got Message: ");
      // something like `threshold-65`
      // split it
      // threshold = newdata;
      Serial.println(message.data());
  });

  pinMode(LED_COLOR_PIN, OUTPUT);
  pinMode(RESISTANCE_PIN, INPUT);
}

void loop() {
  // let the websockets client check for incoming messages
  if(client.available()) {
      client.poll();
  }

  digitalWrite(LED_COLOR_PIN, HIGH);
  reading = analogRead(RESISTANCE_PIN);

  String prefix = "sensor-reading___";
  client.send(prefix + id + "___" + reading);

  if (reading > threshold) {
    // light up power LEDS

    // timer logic
  } else {
    // shut down power LEDs
  }

  delay(100);
}
