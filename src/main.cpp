#include <Arduino.h>
#include <display.h>
#include <ir_sensor.h>
#include <tv_logic.h>
#include <wifi_manager.h>
#include <weather.h>
#include "input.h"
#include <EEPROM.h>

const int RECV_PIN = 3;

uint32_t code;

void setup() {
 // put your setup code here, to run once:
  Serial.begin(115200);

  initDisplay();
  connectWiFi();
  initInput();

  

  initIRsensor(RECV_PIN);

}

void loop(){
 // put your main code here, to run repeatedly:
  
  code = updateIRsensor();
  
  TV_LOGIC(code);
  
  updateDisplay(getCurrentChannel(), getInteractButton());
  // Serial.println(digitalRead(RECV_PIN));


  // Serial.print("X: ");
  // Serial.println(getX());
  // Serial.print("Y: ");
  // Serial.println(getY());
}


