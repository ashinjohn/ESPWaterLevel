/*
   ESP Water Level Controller

   Description: Monitors the water level in my rooftop water tank and operates the water pump
                Also Pushes the water level to Thinkspeak.

   Hardware: WaterLevel -> SR04 -> ESP8266 ->Pump Control -> Thingspeak
                           OLED Display for debugging

   Install ESP8266 library from :http://arduino.esp8266.com/stable/package_esp8266com_index.json
   Install Thingspeak library from : https://github.com/mathworks/thingspeak-arduino
*/
#include "credentails.h"        //This file stores all of the private data like Wifi credentials
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ThingSpeak.h"

// SR04 PIN Mapping
const int SREcho = D7; // D7 connected to Echo pin
const int SRTrig = D6; // D6 connected to Trigger pin

// Hardware Mapping
const int PumpControl = D5;
const int ChargingIndicator = 7; //SO pin of ESP8266
const int FillButton = D4;
const int LevelLED = D8;

// WiFi Configuration
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Thingspeak Configuration
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// OLED Object Creation
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void setup() {
  Serial.begin(115200);  // Initialize serial
  Serial.println("Start");
  // Initialize WiFi Client and ThingSpeak
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  // SR04 PIN Initialisation
  pinMode(SRTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(SREcho, INPUT); // Sets the echoPin as an Input

  //OLED Initialisation
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.clearDisplay(); //Clear splash screen
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("ESP Water Level");
  display.setCursor(0, 0);
  display.display();
}

void loop() {

  // Connect or reconnect to WiFi
  /*
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
        Serial.print(".");
        delay(5000);
      }
      Serial.println("\nConnected.");
    }

    // Write to ThingSpeak. Here, we write to field 1.


    int x = ThingSpeak.writeField(myChannelNumber, 1, Waterlevel(), myWriteAPIKey);
    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    delay(20000); // Wait 20 seconds to update the channel again
  */
}

float Waterlevel() {
  float distance = 0;
  long TimeofFlight = 0; // Variable to store the duaration of TOF of the Ultrasonic pulse

  // Clears the trigger Pin of SR04
  digitalWrite(SRTrig, LOW);
  delayMicroseconds(2);

  // Sets the trigger Pin of SR04 in HIGH state for 10 micro seconds
  digitalWrite(SRTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(SRTrig, LOW);

  // Reads the echo Pin of SR04, returns the sound wave travel time in microseconds
  TimeofFlight = pulseIn(SREcho, HIGH); // pulseIn gets duration until the pin toggles
  distance = TimeofFlight * 0.034 / 2;
  Serial.print("\nWater Level = ");
  Serial.print(distance);
  Serial.println(" cm");
  return (120 - distance); //120cm Tank height subtracted to get water level inside tank
}
