#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "pw";

bool ledState = 0;
const int ledPin = 2;

const int MOTOR_PIN = 18; // 35; //D18;
const int SWITCH_PIN = 4; //24; //D4;
const int LED_PIN = 19; //38; //D19;


void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  ledState = HIGH;
}

void loop() {
  int switch_state = digitalRead(SWITCH_PIN); 
  //Serial.println(switch_state);

  if (switch_state == 0) {
    digitalWrite(LED_PIN, HIGH);  
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(MOTOR_PIN, LOW);  
  }

  digitalWrite(ledPin, ledState);

  //delay(100);
}