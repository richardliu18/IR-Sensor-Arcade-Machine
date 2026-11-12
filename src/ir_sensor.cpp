#include <IRremote.hpp>

void initIRsensor(int RECV_PIN){

    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
    Serial.println("IR Reciever Ready. Press button on remote");
    
}

uint32_t updateIRsensor(){

    if(!IrReceiver.decode()){
        return 0;
    }
    
    IrReceiver.printIRResultShort(&Serial);

    uint32_t value = IrReceiver.decodedIRData.decodedRawData;

    if(value != 0){
        Serial.print("Hex Code: 0x");
        Serial.println(value, HEX);
    }
    
    IrReceiver.resume(); 

    return value;
}