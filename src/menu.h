#pragma once

enum GameState {
    MENU,
    PONG,
    PONGEND,
    SNAKE,
    SNAKEEND,
    BREAKOUT,
    BREAKOUTEND
};

extern GameState currentState;

void updateMenu();
void drawMenu();

