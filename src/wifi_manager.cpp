#include <Arduino.h>
#include <WiFi.h>


const char* ssid = "DeepBlue";
const char* password = "Westford2024";

void connectWiFi(){


    WiFi.mode(WIFI_STA); // wifi station

    WiFi.begin(ssid, password); // connect to wifi

    while (WiFi.status() != WL_CONNECTED){ //wireless connection
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected!");
}

bool isWiFiConnected(){

    return WiFi.status() == WL_CONNECTED;

}

