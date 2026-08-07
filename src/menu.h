#pragma once

enum GameState {
    MENU,
    PONG,
    PONGEND,
    SNAKE,
    FLAPPY
};

extern GameState currentState;

void updateMenu();
void drawMenu();

