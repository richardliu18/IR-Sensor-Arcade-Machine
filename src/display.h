#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


void initDisplay();
void updateDisplay(int channel, bool interactButton);

#endif