#include <Arduino.h>


int xPin;
int yPin;
int buttonPin;

int xVal;
int yVal;
bool buttonState;


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

    buttonState = digitalRead(buttonPin);
    return buttonState;

}