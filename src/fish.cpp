#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "display.h"
#include "fish.h"


int fishX=60;
int fishY=30;
int vx = 1;
int vy = 0;

unsigned long lastThink = 0;
static int frame = 0;
unsigned long lastFrameTime = 0;

bool foodActive = false;
int foodX = 0;
int foodY = 0;

//Fish Frame 1 bitmap
const unsigned char epd_bitmap_fish1left [] PROGMEM = {
    0x00, 0x00, 0x01, 0x8e, 0x03, 0x8a, 0x07, 0xfa, 0x38, 0x01, 0x20, 0x01, 0x48, 0x02, 0x40, 0x02, 
	0x44, 0x02, 0x78, 0x02, 0x40, 0x02, 0x60, 0x32, 0x3f, 0xfe, 0x07, 0xc0, 0x07, 0xc0, 0x03, 0x00
};
const unsigned char epd_bitmap_fish1right [] PROGMEM = {
	0x00, 0x00, 0x71, 0x80, 0x51, 0xc0, 0x5f, 0xe0, 0x80, 0x1c, 0x80, 0x04, 0x40, 0x12, 0x40, 0x02, 
	0x40, 0x22, 0x40, 0x1e, 0x40, 0x02, 0x4c, 0x06, 0x7f, 0xfc, 0x03, 0xe0, 0x03, 0xe0, 0x00, 0xc0
};
const unsigned char epd_bitmap_fish2right [] PROGMEM = {
	0x00, 0x00, 0x07, 0x00, 0xe3, 0x80, 0xbf, 0xe0, 0x80, 0x1c, 0x80, 0x04, 0x80, 0x12, 0xc0, 0x02, 
	0x40, 0x22, 0x40, 0x1e, 0x40, 0x02, 0x4c, 0x06, 0x7f, 0xfc, 0x07, 0xc0, 0x03, 0xc0, 0x03, 0x80
};
const unsigned char epd_bitmap_fish2left [] PROGMEM = {
	0x00, 0x00, 0x00, 0xe0, 0x01, 0xc7, 0x07, 0xfd, 0x38, 0x01, 0x20, 0x01, 0x48, 0x01, 0x40, 0x03, 
	0x44, 0x02, 0x78, 0x02, 0x40, 0x02, 0x60, 0x32, 0x3f, 0xfe, 0x03, 0xe0, 0x03, 0xc0, 0x01, 0xc0
};

void drawFish(int x, int y){
    if(vx<=0){
        if(frame == 0){
            display.drawBitmap( x, y, epd_bitmap_fish1left, 16, 16, SSD1306_WHITE); //going left
        }
        else{
            display.drawBitmap( x, y, epd_bitmap_fish2left, 16, 16, SSD1306_WHITE); //left second frame
        }
        
    }
    else{
        if(frame == 0){
            display.drawBitmap( x, y, epd_bitmap_fish1right, 16, 16, SSD1306_WHITE); //going right
        }
        else{
            display.drawBitmap( x, y, epd_bitmap_fish2right, 16, 16, SSD1306_WHITE); //right second frame
        }
    }
    

    // display.drawRect(fishX, fishY, 10, 10, SSD1306_WHITE);
}

void updateFish(){

    if(foodActive){
        if(foodX>fishX){
            vx=1;
        }
        else if(foodX<fishX){
            vx=-1;
        }
        else{
            vx=0;
        }

        if(foodY>fishY){
            vy=1;
        }
        else if(foodY<fishY){
            vy=-1;
        }
        else{
            vy=0;
        }

    }

    if(foodActive){
        if(abs(fishX - foodX) < 8 && abs(fishY - foodY) < 8){
            foodActive = false;
            vx = 0;
            vy = 0;
        }
}

    if(millis()-lastFrameTime > 300){ // animation frames of fish swimming
        if(frame ==0){
            frame = 1;
        }
        else{
            frame = 0;
        }
        lastFrameTime = millis();
    }

    fishX+=vx;
    fishY+=vy;

    if(fishX<=0 || fishX>=112){
        vx=-vx; // bounce off sides
    }
    if(fishY<=0 || fishY>=48){
        vy=-vy; // bounce off top and bottom
    }
    if(millis() - lastThink > random(2000,4000)){ //random thinking time
        lastThink=millis();

        vx = random(-1,2);
        vy = random(-1,2);
        
    }

}
void spawnFood(){
    foodActive=true;
    foodX=random(10,118);
    foodY=0;
}

void updateFood(){
    if(foodActive){
        foodY+=1;

        if(foodY>56){
            foodActive=false;
        }
    }
}

void drawFood(){
    if(foodActive){
        display.fillCircle(foodX, foodY, 2, SSD1306_WHITE);
    }
}

void showFish(){

    updateFish();
    updateFood();


    drawFish(fishX, fishY);
    drawFood();

    
}

