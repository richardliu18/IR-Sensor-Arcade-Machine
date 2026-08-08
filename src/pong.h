#ifndef PONG_H
#define PONG_H

#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define PADDLE_HEIGHT 15
#define PADDLE_WIDTH 3

struct Paddle {
    int y;
};

struct Ball {
    float x;
    float y;
    float vx;
    float vy;
};

void initPong();
void updatePong();
void drawPong();
int getPlayerScore();
int getAIScore();
void exitPong();
void pongEndScreen();


#endif