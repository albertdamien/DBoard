/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  Simple DHT Humidity & Temperature Demo with DBoard Library (v1.0.0)

*/

#include <DHTSensor.h>

//#define DHT_TYPE DHT11    // DHT 11 
#define DHT_TYPE DHT22      // DHT 22 (AM2302) <- Grove component
//#define DHT_TYPE DHT21    // DHT 21 (AM2301)

DHTSensor dht(A2, DHT22); 

void onEventDht(eventType e) {
    switch (e) {
        case EVENT_READ_READY:
            Serial.print("Humidity: "); 
            Serial.print(dht.humidity());
            Serial.print(" %\t");
            Serial.print("Temperature: "); 
            Serial.print(dht.temperature());
            Serial.println(" °C");
            break;
        case EVENT_READ_ERROR:
            Serial.println("Error reading DHT sensor !");
            break;
    }
} 

void setup() {
    dht.plug(onEventDht);
} 

void loop() {
    Component::loop();
}