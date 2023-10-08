#include <FastLED.h>
#include <ArduinoJson.h>

#define NUM_LEDS 30
#define DATA_PIN 6

//{"ColorRGB":[169, 123, 213]}
byte rgb[3];
DynamicJsonDocument doc(1024);

void setup() {
  Serial.begin(9600);
}

void loop() { 
  if(Serial.available() > 0){

    deserializeJson(doc, Serial.readString());
    
    for(byte i = 0; i<3; i++){  
      rgb[i] = doc["ColorRGB"][i];
    }

    for(int i = 0; i<3; i++){  
      Serial.println(rgb[i]);
    }

  }
}
