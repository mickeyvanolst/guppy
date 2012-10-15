//
//  ardHandler.h
//  lebensRaum
//
//  Created by Mick van Olst on 11/8/12.
//  Copyright (c) 2012 POPSURE. All rights reserved.
//

#ifndef lebensRaum_ardHandler_h
#define lebensRaum_ardHandler_h

#include "ofMain.h"

class ardHandler : ofBaseApp {
    
public:
    ardHandler();
    
    void setup(int serialNumber);
    void update();
    void sendData(char msg);
    
    ofSerial	serial;
        
private:
    
};

#endif
