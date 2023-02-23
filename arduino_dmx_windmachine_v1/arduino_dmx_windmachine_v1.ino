

#include <DmxSimple.h>

// dmx jack
// gnd 1 black
// 2 purple
// 3 green


const int DMX_PIN = 2;

int strength = 0;
int channel = 0;
String data;

void setup() {

  DmxSimple.usePin(DMX_PIN);
  //DmxSimple.maxChannel(4);

  Serial.begin(9600);
  Serial.println("START");
}

void loop() {
  DmxSimple.write(channel, strength); 
  
  while(Serial.available()) {
    data = Serial.readString();
    Serial.println(data);
    // like `01-200`, so channel-strength
      int c = data.substring(0, 1).toInt();
      int p = data.substring(3).toInt();
      strength = p;
      channel = c;
    }
  //delay(100);
}
