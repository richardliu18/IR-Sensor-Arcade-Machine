#include <Arduino.h>
#include "input.h"
#include "pong.h"
#include "display.h"

Paddle debug;



void debugUpdate(){
 //Player input
    display.setTextSize(1);
    display.setCursor(10,10);

    int inputY = getY();

    debug.y=32;
    debug.x=64;

    if (inputY > downThreshold) {        // down
        debug.y += 10;
        display.print("Y: ");
        display.println(getY());
    }
    if (inputY < upThreshold) {        // up
        debug.y -= 10;
        display.print("Y: ");
        display.println(getY());
    }

    int inputX = getX();

    if (inputX > rightThreshold) {        // right
        debug.x += 10;
        display.print("X: ");
        display.println(getX());
    }
    if (inputY < leftThreshold) {        // left
        debug.x -= 10;
        display.print("X: ");
        display.println(getX());
    }

    display.fillCircle(debug.x, debug.y, 10, SSD1306_WHITE);
}

