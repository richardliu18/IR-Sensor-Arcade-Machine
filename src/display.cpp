#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"
#include <fish.h>
#include <dvd.h>
#include <weather.h>
#include <animation.h>
#include <menu.h>
#include <roboEyes.h>
#include "pong.h"
#include "input.h"
#include "pong.h"
#include "snake.h"
#include "debug.h"
#include "breakout.h"





int lastChannel = -999;
bool showTitle = false;
unsigned long titleStartTime = 0;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int currentTime;
bool roboEyesInitialized = false;


void initDisplay(){

    Wire.begin(D4, D5);
    Wire.setClock(400000);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
        Serial.println("Display Offline");
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    currentTime = millis();
    
    display.display();
}

void updateDisplay(int channel, bool interactButton){

    static unsigned long lastFrame = 0;

    if (millis() - lastFrame < 33) {
        return;
    }

    lastFrame = millis();


    if(channel != lastChannel && channel !=-1){
        lastChannel = channel;
        showTitle = true;
        titleStartTime=millis();

        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(5, 20);
        display.setTextSize(2);

        display.print("Channel: ");
        display.println(channel);
        display.display();

        return;
    }
    else if(channel == -1){
        display.clearDisplay();
        display.display();
        return;
    }

    if(showTitle){
        if(millis()-titleStartTime < 1000){
            return;
        }
        else{
            showTitle = false;
        }
    }



    switch(channel){
        case -1:
        display.display();
        break;

        case 0:

        display.clearDisplay();

        showFish();
        if(interactButton || isPressed()){
            Serial.println("Spawn food");
            spawnFood();
        }

        display.display();
        break;

        case 1:
        display.clearDisplay();
        display.setCursor(5,20);
        display.setTextSize(2);
        display.println("Hello!");
        showHelloCat();
        display.display();
        break;

        case 2:
        display.clearDisplay();
        showDVD();
        display.display();
        break;
        
        case 3:
        display.clearDisplay();
        showWeather();
        display.display();
        break;

        case 4:
        display.clearDisplay();
        switch (currentState) {
            case MENU:
                drawMenu();
                break;
            case PONG:
                updatePong();
                drawPong();
                break;
            case PONGEND:
                pongEndScreen();
                break;
            case SNAKE:
                updateSnake();
                drawSnake();
                break;
            case SNAKEEND:
                snakeEndScreen();
                break;
            case BREAKOUT:
                updateBreakout();
                drawBreakout();
                break;
            case BREAKOUTEND:
                breakoutEndScreen();
                break;
    }
        display.display();
        break;

        case 5:
        if (!roboEyesInitialized) {
        initRoboEyes();
        roboEyesInitialized = true;
        }
        if(interactButton){
            roboEyesLaugh();
        }
        updateRoboEyes();
        display.display();
        break;

        case 9:
        display.clearDisplay();
        debugUpdate();
        display.display();
        break;

    }
 


}