#include "reaction.h"
#include "display.h"
#include "menu.h"
#include "input.h"

#include <Adafruit_SSD1306.h>

#include <EEPROM.h>

#define EEPROM_SIZE 8
#define REACTION_BEST_ADDR 4

extern Adafruit_SSD1306 display;



unsigned long bestReactionTime = 0;

enum ReactionPhase{
    WAITING,
    READY,
    RESULT,
    FALSE_START
};

ReactionPhase phase;

unsigned long waitStart;
unsigned long reactionStart;
unsigned long waitDuration;
unsigned long reactionTime;


void startReactionRound(){
    phase = WAITING;

    waitStart = millis();

    waitDuration = random(1500, 4500);

    reactionTime = 0;
}


void initReaction() {


    EEPROM.begin(EEPROM_SIZE);

    bestReactionTime = EEPROM.readULong(REACTION_BEST_ADDR);

    randomSeed(micros()); // make sure "random()" is random

    startReactionRound();
}

// Exit
void exitReaction() {
    currentState = MENU;
}

void updateReaction() {

    bool pressed = isPressed();

    // Exit: hold joystick upper-left and press
    if (pressed &&
        getX() > rightThreshold &&
        getY() < upThreshold) {

        exitReaction();
        return;
    }

    switch (phase) {

        case WAITING:

            // Pressed too early
            if (pressed) {
                phase = FALSE_START;
            }

            // Random delay finished
            else if (millis() - waitStart >= waitDuration) {

                phase = READY;
                reactionStart = millis();
            }

            break;


        case READY:

            // Record reaction time
            if (pressed) {


                reactionTime = millis() - reactionStart;

                //check if highscore
                if (bestReactionTime == 0 || reactionTime < bestReactionTime) {
                bestReactionTime = reactionTime;

                EEPROM.writeULong(REACTION_BEST_ADDR, bestReactionTime);
                EEPROM.commit();
        }

                phase = RESULT;
            }

            break;


        case RESULT:
        case FALSE_START:

            // Press to try again
            if (pressed) {
                startReactionRound();
            }

            break;
    }
}

void drawReaction() {

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(25, 0);
    display.print("REACTION TEST");

    display.drawFastHLine(0, 11, SCREEN_WIDTH, SSD1306_WHITE);

    switch (phase) {

        case WAITING:

            display.setTextSize(1);
            display.setCursor(20, 25);
            display.print("Wait for light...");

            display.drawRect(45, 40, 38, 12, SSD1306_WHITE);

            break;


        case READY:

            display.fillScreen(SSD1306_WHITE);

            display.setTextColor(SSD1306_BLACK);
            display.setTextSize(2);

            display.setCursor(35, 25);
            display.print("GO!");

            break;


        case RESULT:

            display.setTextColor(SSD1306_WHITE);

            display.setTextSize(1);
            display.setCursor(35, 20);
            display.print("YOUR TIME");

            display.setTextSize(2);
            display.setCursor(25, 35);
            display.print(reactionTime);

            display.setTextSize(1);
            display.print(" ms");

            display.setCursor(15, 55);
            display.print("Best: ");
            display.print(bestReactionTime);
            display.print(" ms");


            break;


        case FALSE_START:

            display.setTextSize(1);

            display.setCursor(5, 25);
            display.print("CHEEKY BOY TOO EARLY");

            display.setCursor(15, 45);
            display.print("Press to retry");

            break;
    }
}
