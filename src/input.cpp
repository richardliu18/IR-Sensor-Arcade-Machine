#include <Arduino.h>


int xPin = 7;
int yPin = 1;
int buttonPin = 9;

int xVal;
int yVal;
bool buttonState;

static bool lastState = HIGH;


void initInput(){
    
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);

}
int getX(){

    xVal = analogRead(xPin);
    return xVal;
}

int getY(){
    
    yVal = analogRead(yPin);
    return yVal;

}

bool isPressed(){

    static bool lastState = HIGH;

    bool currentState = digitalRead(buttonPin);

    if(lastState == HIGH && currentState == LOW){
        lastState = currentState;
        return true;
    }

    lastState = currentState;
    return false;

}