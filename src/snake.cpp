#include "snake.h"
#include "display.h"
#include "input.h"
#include "menu.h" 
#include <EEPROM.h>

#define SCORE_ADDR 0

int highScore = EEPROM.read(SCORE_ADDR);


const int grid_size = 8;
const int max_length = 100;

const int screen_width = 128;
const int screen_height = 64;

const int GRID_W = screen_width / grid_size;
const int GRID_H = screen_height / grid_size;

int snakeX[max_length];
int snakeY[max_length];
int length;

int dirX, dirY;

int appleX, appleY;

void initSnake(){

    EEPROM.begin(512);

    EEPROM.get(SCORE_ADDR, highScore);//highscore EEPROM


    length = 3;

    snakeX[0] = GRID_W/2; //Head of snake
    snakeY[0] = GRID_H/2;

    snakeX[1] = snakeX[0] - 1;
    snakeY[1] = snakeY[0];

    snakeX[2] = snakeX[1] - 1;
    snakeY[2] = snakeY[1];

    dirX = 1;
    dirY = 0;

    // Random food
    appleX = random(0, GRID_W);
    appleY = random(0, GRID_H);

}

void handleInput(){
    int x = getX();
    int y = getY();

    // Prevent reversing into itself
    if (x > rightThreshold && dirX != 1) {      // right
        dirX = -1;
        dirY = 0;
    }
    else if (x < leftThreshold && dirX != -1) { // left
        dirX = 1;
        dirY = 0;
    }
    else if (y < upThreshold && dirY != 1) {  // up
        dirX = 0;
        dirY = -1;
    }
    else if (y > downThreshold && dirY != -1) { // down
        dirX = 0;
        dirY = 1;
    }
}
void updateSnake() {

    handleInput();

    if (isPressed()) {
        currentState = MENU;
        return;
    }

    // Move body
    for (int i = length - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    // Move head
    snakeX[0] += dirX;
    snakeY[0] += dirY;

    // Wall collision
    if (snakeX[0] < 0 || snakeX[0] >= GRID_W ||
        snakeY[0] < 0 || snakeY[0] >= GRID_H) {
        currentState = SNAKEEND;
    }

    // Self collision
    for (int i = 1; i < length; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            currentState = SNAKEEND;
        }
    }

    // Food collision
    if (snakeX[0] == appleX && snakeY[0] == appleY) {
        
        if (length < max_length) length++;

        appleX = random(0, GRID_W);
        appleY = random(0, GRID_H);
    }

    delay(120);
}

void drawSnake() {

    // Draw snake
    for (int i = 0; i < length; i++) {
        display.fillRect(
            snakeX[i] * grid_size,
            snakeY[i] * grid_size,
            grid_size,
            grid_size,
            WHITE
        );
    }

    // Draw food
    display.fillRect(
        appleX * grid_size,
        appleY * grid_size,
        grid_size,
        grid_size,
        WHITE
    );
    display.setTextSize(1);
    display.setCursor(45, 0);
    display.print("SNAKE");
   
}
void snakeEndScreen(){

    if (isPressed()) {
        currentState = MENU;
        return;
    }

    //check and update highscore
    if (length > highScore) {
    highScore = length;
    EEPROM.put(SCORE_ADDR, highScore);
    EEPROM.commit();

}
    
     // Draw scores
    display.setTextSize(2);
    display.setTextColor(WHITE);

    // Player score (left)
    display.setCursor(0, 0);
    display.print("Length:");
    display.print(length);

    display.setCursor(0, 30);
    display.print("Highscore:");
    display.print(highScore);




}

