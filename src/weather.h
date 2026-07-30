#ifndef WEATHER_H
#define WEATHER_H
#include <Arduino.h>

struct WeatherData {
    float temp;
    String condition;
};

void updateWeather();
WeatherData getWeatherData();
void showWeather();
void showWeatherAnimation(String condition);

#endif