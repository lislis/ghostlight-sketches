

#include <DmxSimple.h>

// dmx jack
// gnd 1 black
// 2 purple
// 3 green


const int DMX_PIN = 2;

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

int strength = 200;
int channel = 0;
String data;

void setup() {

  DmxSimple.usePin(DMX_PIN);
  //DmxSimple.maxChannel(4);

  Serial.begin(9600);
  Serial.println("START");
  //Serial.begin(115200);
}

void loop() {
  DmxSimple.write(channel_red, red_strength); 
  DmxSimple.write(channel_green, green_strength); 
  DmxSimple.write(channel_blue, blue_strength); 
  DmxSimple.write(channel_wind, wind_strength); 
  DmxSimple.write(channel_hallway, hallway_strength); 
  
  while(Serial.available()) {
    data = Serial.readString();
    Serial.println(data);
    // like `01-200`, so channel-strength
      int c = data.substring(0, 1).toInt();
      int p = data.substring(2).toInt();

      if (c == 1) {
        red_strength = p;
      }

      if (c == 2) {
        green_strength = p;
      }

      if (c == 3) {
        blue_strength = p;
      }

      if (c == 4) {
        wind_strength = p;
      }

      if (c == 5) {
        hallway_strength = p;
      }
      Serial.println(c);
      Serial.println(p);
      strength = p;
      channel = c;
    }
  //delay(100);
}
