#include "menu.h"
#include "display.h"
#include "input.h"
#include "pong.h"

int selected = 0;
const int upThreshold = 1000;
const int downThreshold = 4000;
const int neutralLow = 1800;
const int neutralHigh = 2100;

const char* items[] = {
    "GAME MENU",
    "Pong",
    "Flappy Bird"
};

GameState currentState = MENU;

int menuSize = 3;

void updateMenu() {

    int y = getY();

    if (y > downThreshold) {        // down
        selected++;
        delay(150);
    }
    if (y < upThreshold) {        // up
        selected--;
        delay(150);
    }

    if (selected < 0) selected = menuSize - 1;

    if (selected >= menuSize) selected = 0;
    
    if (isPressed()) {

        if(selected==1){
            currentState = PONG;
            initPong();
        }
    }
}

void drawMenu() {

    updateMenu();
    
    display.setTextSize(1.5);
    
    for (int i = 0; i < menuSize; i++) {
        if (i == selected) {
            display.setCursor(0, i * 10);
            display.print(">");
        }

        display.setCursor(10, i * 10);
        display.print(items[i]);
    }
}