#include "pong.h"
#include "input.h"
#include "display.h"
#include <Adafruit_SSD1306.h>
#include "menu.h"

extern Adafruit_SSD1306 display;

Paddle player;
Paddle ai;
Ball ball;

int playerScore = 0;
int aiScore = 0;



#define AI_SPEED 1   // lower = easier, higher = harder

void initPong() {

    player.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    ai.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;

    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.vx = 2.2;
    ball.vy = 0.8;
}
void updatePong() {

    if (isPressed()) {
        exitPong();
        return;
    }

    if(aiScore >= 10 || playerScore >= 10){
        currentState = PONGEND;
        return;
    }

    //Player input
    int input = getY();

    if (input > downThreshold) {        // down
        player.y += 2;
    }
    if (input < upThreshold) {        // up
        player.y -= 2;
    }

    if (player.y < 0) player.y = 0;
    if (player.y > SCREEN_HEIGHT - PADDLE_HEIGHT)
        player.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

    //AI Movement
    int target = ball.y - PADDLE_HEIGHT / 2;

    if (ai.y < target) ai.y += AI_SPEED;
    else if (ai.y > target) ai.y -= AI_SPEED;

    // Boundaries
    if (ai.y < 0) ai.y = 0;
    if (ai.y > SCREEN_HEIGHT - PADDLE_HEIGHT)
        ai.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

    //Ball
    ball.x += ball.vx;
    ball.y += ball.vy;

    // Top/bottom bounce
    if (ball.y <= 0) {
        ball.y = 0;
        ball.vy *= -1;
    }

    if (ball.y >= SCREEN_HEIGHT - 2) {
        ball.y = SCREEN_HEIGHT - 2;
        ball.vy *= -1;
    }

    // Player paddle(left)
    if (ball.x <= PADDLE_WIDTH) {
        if (ball.y >= player.y && ball.y <= player.y + PADDLE_HEIGHT) {

            ball.vx *= -1;
            ball.vx *= 1.02;//increase ball speed with hit
            ball.vy *= 1.02;  

            float hitPosition = 
                (ball.y - (player.y + PADDLE_HEIGHT / 2)) 
                / (PADDLE_HEIGHT / 2.0);

            ball.vy = hitPosition * 2.5;

            // Prevent straight shots
            if (abs(ball.vy) < 0.5) {
                ball.vy = (random(0, 2) == 0) ? -1 : 1;
            }
        }
}

    // AI paddle(right)
    if (ball.x >= SCREEN_WIDTH - PADDLE_WIDTH - 1) {
        if (ball.y >= ai.y && ball.y <= ai.y + PADDLE_HEIGHT) {

            ball.vx *= -1;

            float hitPosition =
                (ball.y - (ai.y + PADDLE_HEIGHT / 2))
                / (PADDLE_HEIGHT / 2.0);

            ball.vy = hitPosition * 2.5;

            // Prevent straight shots
            if (abs(ball.vy) < 0.5) {
                ball.vy = (random(0, 2) == 0) ? -1 : 1;
            }
        }
}

    // Score update
    if (ball.x < 0) {
        aiScore++;   // AI scores
        ball.vx=2;
    }
    else if (ball.x > SCREEN_WIDTH) {
        playerScore++; // Player scores
        ball.vx=-2;
    }

    if (ball.x < 0 || ball.x > SCREEN_WIDTH) {
        ball.x = SCREEN_WIDTH / 2;
        ball.y = SCREEN_HEIGHT / 2;


}
}

void drawPong(){
    // Player paddle (left)
    display.fillRect(0, player.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

    // AI paddle (right)
    display.fillRect(SCREEN_WIDTH - PADDLE_WIDTH, ai.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

    // Ball
    display.fillRect(ball.x, ball.y, 2, 2, WHITE);
    
    // Draw scores
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Player score (left)
    display.setCursor(20, 0);
    display.print(playerScore);

    // AI score (right)
    display.setCursor(SCREEN_WIDTH - 30, 0);
    display.print(aiScore);

    
}
int getPlayerScore() {
    return playerScore;
}

int getAIScore() {
    return aiScore;
}
void exitPong() {
    currentState = MENU;
    ball.x = 64;
    ball.y = 32;
    playerScore = 0;
    aiScore = 0;
}
void pongEndScreen(){

    if (isPressed()) {
        exitPong();
        return;
    }
    
     // Draw scores
    display.setTextSize(3);
    display.setTextColor(WHITE);

    // Player score (left)
    display.setCursor(20, 30);
    display.print(playerScore);

    // AI score (right)
    display.setCursor(SCREEN_WIDTH - 50, 30);
    display.print(aiScore);

    display.setCursor(55, 30);
    display.print("-");

    if(aiScore > playerScore){
        display.setTextSize(2);
        display.setCursor(20, 0);
        display.print("You lose");
    }
    else{
        display.setTextSize(2);
        display.setCursor(25, 0);
        display.print("You win");
    }
}

