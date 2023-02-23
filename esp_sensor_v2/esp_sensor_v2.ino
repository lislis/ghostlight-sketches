/*
  ESP32 sensor v2.2
  - Connects to websocket
  - Sends out id as registration
  - Reads resistance and sends it
  - Receives threshold info
  - Receives interval info
  - Lights a row of LEDs in sensor color
  - Indicates threshold crossing through lighting up another row of LEDs
  
  Where the LED_TRIGGERs are individually triggerable to indicate status
  And LED_COLOR_PIN triggers a transistor to light up the color indicator LEDs

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
String id = "sensorRED";

using namespace websockets;

const int RESISTANCE_PIN = A0; // AC1_0 aka GPIO36 aka VP
const int LED_COLOR_PIN = 4; 
const int LED_TRIGGER_1 = 18;
const int LED_TRIGGER_2 = 19;
const int LED_TRIGGER_3 = 21;

int reading = 0;
int threshold = 100;

bool is_reading = false;
bool is_active = false;
unsigned long start_time = 0;
unsigned long current_time = 0;
int interval = 4000;

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
      //Serial.print("Got Message: ");
      Serial.println(message.data());
      // something like `threshold-65`
      if (message.data().substring(0,9) == "threshold") {
        //Serial.print(message.data().substring(0,9));
        //Serial.println(message.data().substring(10).toInt());
        threshold = message.data().substring(10).toInt();
      }
      // like `interval-3000`
      if (message.data().substring(0,8) == "interval") {
        interval = message.data().substring(9).toInt();
      }
      // like `active-0`
      if (message.data() == "active-true") {
        is_active = true;
      }
      if (message.data() == "active-false") {
        is_active = false;
      }
  });

  pinMode(LED_COLOR_PIN, OUTPUT);
  pinMode(LED_TRIGGER_1, OUTPUT);
  pinMode(LED_TRIGGER_2, OUTPUT);
  pinMode(LED_TRIGGER_3, OUTPUT);
  pinMode(RESISTANCE_PIN, INPUT);
}

void loop() {
  // let the websockets client check for incoming messages
  if(client.available()) {
      client.poll();
  }

  if (is_active == true) {
    digitalWrite(LED_COLOR_PIN, HIGH);


    if (reading >= threshold && is_reading == false) {
      is_reading = true;
      start_time = millis();
    }
    if (reading < threshold) {
      //Serial.print("FALSE");
      is_reading = false;
      digitalWrite(LED_TRIGGER_1, LOW);
      digitalWrite(LED_TRIGGER_2, LOW);
      digitalWrite(LED_TRIGGER_3, LOW);
    }

    if (is_reading == true) {
      current_time = millis();
    }

    if (current_time - start_time >= 300 
    && is_reading == true) {
      digitalWrite(LED_TRIGGER_1, HIGH);
      //Serial.print("FIRST LIGHT");
    }

    if (current_time - start_time >= (interval + 300) 
    && is_reading == true) {
      digitalWrite(LED_TRIGGER_2, HIGH);
      //Serial.print("2nd LIGHT");
    }

    if (current_time - start_time >= (300 + (interval * 2)) 
    && is_reading == true) {
      digitalWrite(LED_TRIGGER_3, HIGH);
      //Serial.print("3rd LIGHT");
      client.send("sensor-triggered___" + id);
    }
  } else {
    digitalWrite(LED_COLOR_PIN, LOW);
    digitalWrite(LED_TRIGGER_1, LOW);
    digitalWrite(LED_TRIGGER_2, LOW);
    digitalWrite(LED_TRIGGER_3, LOW);
  }
  
  reading = analogRead(RESISTANCE_PIN);

  String prefix = "sensor-reading___";
  client.send(prefix + id + "___" + reading);

  delay(500);
}
