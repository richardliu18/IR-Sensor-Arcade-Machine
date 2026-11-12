#pragma once

enum GameState {
    MENU,
    PONG,
    PONGEND,
    SNAKE,
    SNAKEEND,
    BREAKOUT,
    BREAKOUTEND,
    REACTION,
    REACTIONEND,
    SKETCH,
    SKETCHEND,
};

extern GameState currentState;

void updateMenu();
void drawMenu();

