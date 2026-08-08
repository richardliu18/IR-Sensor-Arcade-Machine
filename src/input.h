#pragma once

const int upThreshold = 1000;
const int downThreshold = 4000;
const int leftThreshold = 1000;
const int rightThreshold = 4000;
const int neutralLow = 1800;
const int neutralHigh = 2100;


void initInput();
int getX();
int getY();
bool isPressed();