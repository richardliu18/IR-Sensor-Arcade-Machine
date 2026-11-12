#include "sketch.h"
#include "display.h"
#include "menu.h"
#include "input.h"

#include <Adafruit_SSD1306.h>
#include <string.h>

extern Adafruit_SSD1306 display;

#define CANVAS_START_Y 12
#define CURSOR_SPEED 2

static uint8_t canvas[SCREEN_WIDTH * SCREEN_HEIGHT / 8];

static int cursorX;
static int cursorY;
static int previousCursorX;
static int previousCursorY;

static bool drawing = false;

// Set a pixel on the canvas
void setCanvasPixel(int x, int y) {

    if (x < 0 || x >= SCREEN_WIDTH ||
        y < CANVAS_START_Y || y >= SCREEN_HEIGHT) {
        return;
    }

    int index = y * SCREEN_WIDTH + x;

    canvas[index / 8] |= (1 << (index % 8));
}

// Check whether a pixel is drawn
bool getCanvasPixel(int x, int y) {

    int index = y * SCREEN_WIDTH + x;

    return canvas[index / 8] & (1 << (index % 8));
}

// Draw a line between two points
void drawCanvasLine(int x0, int y0, int x1, int y1) {

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int error = dx - dy;

    while (true) {

        setCanvasPixel(x0, y0);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int error2 = 2 * error;

        if (error2 > -dy) {
            error -= dy;
            x0 += sx;
        }

        if (error2 < dx) {
            error += dx;
            y0 += sy;
        }
    }
}

void initSketch() {

    memset(canvas, 0, sizeof(canvas));

    cursorX = SCREEN_WIDTH / 2;
    cursorY = (SCREEN_HEIGHT + CANVAS_START_Y) / 2;

    previousCursorX = cursorX;
    previousCursorY = cursorY;

    drawing = false;
}

void updateSketch() {

    bool pressed = isPressed();

    // Exit: joystick upper-left + button
    if (pressed &&
        getX() > rightThreshold &&
        getY() < upThreshold) {

        exitSketch();
        return;
    }

    // Toggle drawing
    if (pressed) {
        drawing = !drawing;
    }

    previousCursorX = cursorX;
    previousCursorY = cursorY;

    int xInput = getX();
    int yInput = getY();

    if (xInput > rightThreshold) cursorX -= CURSOR_SPEED;
    if (xInput < leftThreshold) cursorX += CURSOR_SPEED;

    if (yInput < upThreshold) cursorY -= CURSOR_SPEED;
    if (yInput > downThreshold) cursorY += CURSOR_SPEED;

    cursorX = constrain(cursorX, 0, SCREEN_WIDTH - 1);
    cursorY = constrain(cursorY, CANVAS_START_Y + 1, SCREEN_HEIGHT - 2);

    if (drawing) {
        drawCanvasLine(
            previousCursorX,
            previousCursorY,
            cursorX,
            cursorY
        );
    }
}

void drawSketch() {

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(2, 2);
    display.print("SKETCH");

    display.setCursor(78, 2);
    display.print(drawing ? "DRAW ON" : "DRAW OFF");

    display.drawFastHLine(
        0,
        CANVAS_START_Y - 1,
        SCREEN_WIDTH,
        SSD1306_WHITE
    );

    // Draw saved canvas pixels
    for (int y = CANVAS_START_Y; y < SCREEN_HEIGHT; y++) {

        for (int x = 0; x < SCREEN_WIDTH; x++) {

            if (getCanvasPixel(x, y)) {
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        }
    }

    // Draw cursor
    display.drawRect(
        cursorX - 1,
        cursorY - 1,
        3,
        3,
        SSD1306_WHITE
    );
}

void exitSketch() {
    currentState = MENU;
}