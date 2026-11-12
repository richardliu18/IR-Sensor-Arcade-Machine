
#include "breakout.h"
#include "input.h"
#include "display.h"
#include <Adafruit_SSD1306.h>
#include "menu.h"
#include "display.h"
#include <math.h>

extern Adafruit_SSD1306 display;

// Game settings
#define BALL_SIZE 2

#define PADDLE_WIDTH 22
#define PADDLE_HEIGHT 3
#define PADDLE_SPEED 3

#define BRICK_ROWS 4
#define BRICK_COLS 10

#define BRICK_WIDTH 11
#define BRICK_HEIGHT 4
#define BRICK_GAP 1

#define BRICK_START_Y 12

#define INITIAL_BALL_SPEED 1
#define MAX_BALL_SPEED 3

// Ball
struct BreakoutBall {
    float x;
    float y;
    float vx;
    float vy;
};

// Paddle
struct BreakoutPaddle {
    int x;
    int y;
};

// Bricks
struct Brick {
    int x;
    int y;
    bool active;
};

static BreakoutBall ball;
BreakoutPaddle paddle;

Brick bricks[BRICK_ROWS][BRICK_COLS];

int score = 0;
int lives = 3;

bool gameWon = false;

// Previous ball position
float previousX;
float previousY;


// ------------------------------------
// INITIALIZE GAME
// ------------------------------------

void initBreakout() {

    score = 0;
    lives = 3;
    gameWon = false;

    // Initialize paddle
    paddle.x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    paddle.y = SCREEN_HEIGHT - 7;

    // Initialize ball
    ball.x = SCREEN_WIDTH / 2;
    ball.y = paddle.y - BALL_SIZE - 1;

    ball.vx = INITIAL_BALL_SPEED;
    ball.vy = -INITIAL_BALL_SPEED;

    // Create brick grid
    int totalWidth =
        BRICK_COLS * BRICK_WIDTH +
        (BRICK_COLS - 1) * BRICK_GAP;

    int startX = (SCREEN_WIDTH - totalWidth) / 2;

    for (int row = 0; row < BRICK_ROWS; row++) {

        for (int col = 0; col < BRICK_COLS; col++) {

            bricks[row][col].x =
                startX + col * (BRICK_WIDTH + BRICK_GAP);

            bricks[row][col].y =
                BRICK_START_Y + row * (BRICK_HEIGHT + 2);

            bricks[row][col].active = true;
        }
    }
}


// ------------------------------------
// RESET BALL
// ------------------------------------

void resetBall() {

    ball.x = paddle.x + PADDLE_WIDTH / 2;
    ball.y = paddle.y - BALL_SIZE - 1;

    ball.vx = (random(0, 2) == 0)
        ? -INITIAL_BALL_SPEED
        : INITIAL_BALL_SPEED;

    ball.vy = -INITIAL_BALL_SPEED;
}


// ------------------------------------
// CHECK BRICK COLLISIONS
// ------------------------------------

void checkBrickCollisions() {

    for (int row = 0; row < BRICK_ROWS; row++) {

        for (int col = 0; col < BRICK_COLS; col++) {

            Brick &brick = bricks[row][col];

            if (!brick.active) {
                continue;
            }

            // Check rectangle overlap
            bool collision =
                ball.x + BALL_SIZE > brick.x &&
                ball.x < brick.x + BRICK_WIDTH &&
                ball.y + BALL_SIZE > brick.y &&
                ball.y < brick.y + BRICK_HEIGHT;

            if (collision) {

                brick.active = false;
                score += 10;

                // Determine collision direction
                float oldRight = previousX + BALL_SIZE;
                float oldBottom = previousY + BALL_SIZE;

                bool hitFromLeft =
                    oldRight <= brick.x;

                bool hitFromRight =
                    previousX >= brick.x + BRICK_WIDTH;

                bool hitFromTop =
                    oldBottom <= brick.y;

                bool hitFromBottom =
                    previousY >= brick.y + BRICK_HEIGHT;

                if (hitFromLeft || hitFromRight) {
                    ball.vx *= -1;
                }
                else if (hitFromTop || hitFromBottom) {
                    ball.vy *= -1;
                }
                else {
                    ball.vy *= -1;
                }

                return;
            }
        }
    }
}


// ------------------------------------
// UPDATE GAME
// ------------------------------------

void updateBreakout() {

    // Exit game
    if (isPressed()) {
        exitBreakout();
        return;
    }

    // Check win condition
    int remainingBricks = 0;

    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {

            if (bricks[row][col].active) {
                remainingBricks++;
            }
        }
    }

    if (remainingBricks == 0) {
        gameWon = true;
        currentState = BREAKOUTEND;
        return;
    }

    // --------------------------------
    // PLAYER MOVEMENT
    // --------------------------------

    int input = getX();

    if (input < leftThreshold) {
        paddle.x += PADDLE_SPEED;
    }

    if (input > rightThreshold) {
        paddle.x -= PADDLE_SPEED;
    }

    // Keep paddle inside screen
    if (paddle.x < 0) {
        paddle.x = 0;
    }

    if (paddle.x > SCREEN_WIDTH - PADDLE_WIDTH) {
        paddle.x = SCREEN_WIDTH - PADDLE_WIDTH;
    }

    // --------------------------------
    // BALL MOVEMENT
    // --------------------------------

    previousX = ball.x;
    previousY = ball.y;

    ball.x += ball.vx;
    ball.y += ball.vy;

    // Left wall
    if (ball.x <= 0) {
        ball.x = 0;
        ball.vx = fabsf(ball.vx);
    }

    // Right wall
    if (ball.x >= SCREEN_WIDTH - BALL_SIZE) {
        ball.x = SCREEN_WIDTH - BALL_SIZE;
        ball.vx = -fabsf(ball.vx);
    }

    // Top wall
    if (ball.y <= 0) {
        ball.y = 0;
        ball.vy = fabsf(ball.vy);
    }

    // --------------------------------
    // PADDLE COLLISION
    // --------------------------------

    bool paddleCollision =
        ball.vy > 0 &&
        ball.x + BALL_SIZE > paddle.x &&
        ball.x < paddle.x + PADDLE_WIDTH &&
        ball.y + BALL_SIZE >= paddle.y &&
        previousY + BALL_SIZE <= paddle.y;

    if (paddleCollision) {

        ball.y = paddle.y - BALL_SIZE;

        // Calculate where the ball hit the paddle
        float paddleCenter =
            paddle.x + PADDLE_WIDTH / 2.0;

        float hitPosition =
            (ball.x + BALL_SIZE / 2.0 - paddleCenter)
            / (PADDLE_WIDTH / 2.0);

        // Change ball direction
        ball.vx = hitPosition * 2.5;

        if (ball.vx > 0 && ball.vx < 0.5) {
            ball.vx = 0.5;
        }

        if (ball.vx < 0 && ball.vx > -0.5) {
            ball.vx = -0.5;
        }

        ball.vy = -fabsf(ball.vy);

        // Increase speed slightly
        ball.vx *= 1.02;
        ball.vy *= 1.02;

        // Speed limit
        if (fabsf(ball.vx) > MAX_BALL_SPEED) {
            ball.vx = (ball.vx > 0)
                ? MAX_BALL_SPEED
                : -MAX_BALL_SPEED;
        }

        if (fabsf(ball.vy) > MAX_BALL_SPEED) {
            ball.vy = -MAX_BALL_SPEED;
        }
    }

    // --------------------------------
    // BRICK COLLISION
    // --------------------------------

    checkBrickCollisions();

    // --------------------------------
    // PLAYER LOSES A LIFE
    // --------------------------------

    if (ball.y > SCREEN_HEIGHT) {

        lives--;

        if (lives <= 0) {
            gameWon = false;
            currentState = BREAKOUTEND;
            return;
        }

        resetBall();
    }
}


// ------------------------------------
// DRAW GAME
// ------------------------------------

void drawBreakout() {

    // Draw bricks
    for (int row = 0; row < BRICK_ROWS; row++) {

        for (int col = 0; col < BRICK_COLS; col++) {

            Brick &brick = bricks[row][col];

            if (brick.active) {

                display.fillRect(
                    brick.x,
                    brick.y,
                    BRICK_WIDTH,
                    BRICK_HEIGHT,
                    WHITE
                );
            }
        }
    }

    // Draw paddle
    display.fillRect(
        paddle.x,
        paddle.y,
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
        WHITE
    );

    // Draw ball
    display.fillRect(
        (int)ball.x,
        (int)ball.y,
        BALL_SIZE,
        BALL_SIZE,
        WHITE
    );

    // Draw score
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(2, 0);
    display.print("S:");
    display.print(score);

    // Draw lives
    display.setCursor(SCREEN_WIDTH - 43, 0);
    display.print("L:");
    display.print(lives);

    // Separator
    display.drawFastHLine(
        0,
        9,
        SCREEN_WIDTH,
        WHITE
    );
}


// ------------------------------------
// EXIT GAME
// ------------------------------------

void exitBreakout() {

    currentState = MENU;

    score = 0;
    lives = 3;

    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
}


// ------------------------------------
// END SCREEN
// ------------------------------------

void breakoutEndScreen() {

    if (isPressed()) {
        exitBreakout();
        return;
    }

    display.setTextColor(WHITE);

    display.setTextSize(2);

    if (gameWon) {
        display.setCursor(15, 5);
        display.print("YOU WIN!");
    }
    else {
        display.setCursor(15, 5);
        display.print("GAME OVER");
    }

    display.setTextSize(1);

    display.setCursor(25, 32);
    display.print("Score: ");
    display.print(score);

    display.setCursor(18, 50);
    display.print("Press to exit");
}


// ------------------------------------
// GETTERS
// ------------------------------------

int getBreakoutScore() {
    return score;
}

int getBreakoutLives() {
    return lives;
}
