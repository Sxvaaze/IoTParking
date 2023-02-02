#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>;
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP32_Servo.h> 

const char *ssid =  "";     // Enter your WiFi Name
const char *pass =  ""; // Enter your WiFi Password

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "your-username"
#define MQTT_PASS "your-key"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int slots[5] {25,33,32,27,26};

int CLOSE_ANGLE = 80;  // The closing angle of the servo motor arm
int OPEN_ANGLE = 0;  // The opening angle of the servo motor arm  

int previous_slots[5] = {0,0,0,0,0};

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're publishing to
Adafruit_MQTT_Publish CarsParked = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/CarsParked");
Adafruit_MQTT_Publish Slot1 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Slot1");
Adafruit_MQTT_Publish Slot2 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Slot2");
Adafruit_MQTT_Publish Slot3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Slot3");
Adafruit_MQTT_Publish EntrySlot = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntryGate");
Adafruit_MQTT_Publish ExitSlot = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitGate");
Adafruit_MQTT_Publish EntryTime = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntryTime");
Adafruit_MQTT_Publish ExitTime = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitTime");

Servo servo1,servo2;
String t;

void publishIR(Adafruit_MQTT_Publish Feed, int slot_pos, int pos) {
    int state = digitalRead(slots[slot_pos]);
    if (state == previous_slots[pos]) {
        return;
    }

    previous_slots[pos] = state;
    if (state == LOW) {
        timeClient.update();
        t = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());
        if (pos == 3) {
            EntryTime.publish((char *)t.c_str());
            servo1.write(CLOSE_ANGLE);
            delay(3000);
            servo1.write(OPEN_ANGLE);
        } else if (pos == 4) {
            ExitTime.publish((char *)t.c_str());
            servo2.write(CLOSE_ANGLE);
            delay(3000);
            servo2.write(OPEN_ANGLE);
        } else {
            Feed.publish("1");
        }
    }
    else {
        Feed.publish("0");
    }
    
    delay(2000);
}

void setup() {
    // initialize serial communication at 9600 bits per second:
    delay(1000);
    Serial.begin(9600); 

    for (int i = 0; i < 5; i++) {
        pinMode(slots[i], INPUT);
    }

    servo1.attach(4);
    servo2.attach(5);

    WiFi.begin(ssid, pass);            
    Serial.print("Connecting to ");
    Serial.print(ssid);     
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");                  
        delay(500);
    }

    timeClient.begin(); 
    timeClient.setTimeOffset(7200);
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP Address is : ");
    Serial.println(WiFi.localIP()); 
}

void loop() {
    MQTT_connect();
    
    publishIR(Slot1, 0, 0);
    publishIR(Slot2, 1, 1);
    publishIR(Slot3, 2, 2);
    publishIR(EntrySlot, 3, 3);
    publishIR(ExitSlot, 4, 4);
}

void MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected()) {
        return;
    }

    uint8_t retries = 3;

    while ((ret = mqtt.connect()) != 0) {// connect will return 0 for connected
        Serial.println("MQTT: Trying again!");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            // basically die and wait for WDT to reset me
            while (1);
        }
    }
}