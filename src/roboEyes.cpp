#include <FluxGarage_RoboEyes.h>
#include "display.h"


RoboEyes<Adafruit_SSD1306> roboEyes(display); // create RoboEyes instance
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

void initRoboEyes(){
    roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);
    roboEyes.setDisplayColors(0, 1);    // background, eye color
    roboEyes.setAutoblinker(true, 3, 5);
    roboEyes.setIdleMode(true, 2, 3);
    roboEyes.setMood(HAPPY);//HAPPY, ANGRY, TIRED, DEFAULT
    roboEyes.setCuriosity(true);
}
void updateRoboEyes(){
    roboEyes.update();
}
void setRoboEyes(unsigned char mood, bool idleMode, bool autoBlinker, bool curiosity, bool cyclops, bool sweat){
    roboEyes.setMood(mood);
    roboEyes.setIdleMode(idleMode, 2, 3);
    roboEyes.setAutoblinker(autoBlinker, 3, 5);
    roboEyes.setCuriosity(curiosity);
    roboEyes.setCyclops(cyclops);
    roboEyes.setSweat(sweat);
}
void roboEyesConfused(){
    roboEyes.anim_confused();
}
void roboEyesLaugh(){
    roboEyes.anim_laugh();
}




