#pragma once

enum GameState {
    MENU,
    PONG,
    PONGEND,
    SNAKE,
    SNAKEEND,
    FLAPPY
};

extern GameState currentState;

void updateMenu();
void drawMenu();

