#include "weather.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "wifi_manager.h"
#include "display.h"
#include "animation.h"

WeatherData currentWeather;

unsigned long lastUpdate = 0;

void updateWeather(){

    if(!isWiFiConnected()) return; //check wifi

    if(millis() - lastUpdate < 20000) return; //update every 20 seconds

    HTTPClient http; //object to interact with web

    String url = "http://api.openweathermap.org/data/2.5/weather?q=Boston&appid=45448533afc8558d22fe2acdbebe7fa7&units=imperial";

    http.begin(url);

    int httpCode = http.GET();

    if(httpCode == 200){ //200 = success, 401 = bad API key, 404 = not found

        String data = http.getString(); //retrieve weather data from OpenWeatherMap API

        DynamicJsonDocument doc(1024); //create a Json object to store info (upto 1024 bytes)

        deserializeJson(doc, data); //add weather data

        currentWeather.temp = doc["main"]["temp"].as<float>(); //parse doc to get and set info
        currentWeather.condition = doc["weather"][0]["main"].as<String>();
    }

    http.end();
    lastUpdate = millis();
}

WeatherData getWeatherData(){
    return currentWeather;
}

void showWeather(){

    updateWeather();

    display.setTextSize(1);
    display.setCursor(0,0);

    display.println("Weather");

    display.print("Temp: ");
    display.print(currentWeather.temp);
    display.println(" F");

    display.print("Conditions: ");
    display.println(currentWeather.condition);

    showWeatherAnimation(currentWeather.condition);
}

void showWeatherAnimation(String condition){

    if(condition.equals("Clouds")){
        showClouds();
    }
    if(condition.equals("Rain")){
        showRain();
    }
    else{
        return;
    }


}