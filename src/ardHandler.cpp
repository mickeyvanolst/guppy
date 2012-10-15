//
//  ardHandler.cpp
//  lebensRaum
//
//  Created by Mick van Olst on 11/8/12.
//  Copyright (c) 2012 POPSURE. All rights reserved.
//
//

#include "ardHandler.h"

//--------------------------------------------------------------
ardHandler::ardHandler()
{
    
}

//--------------------------------------------------------------
void ardHandler::setup(int serialNumber)
{
    serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	serial.setup(serialNumber, 9600); //open the first device
	//serial.setup("COM4"); // windows example
	//serial.setup("/dev/tty.usbserial-A4001JEC",9600); // mac osx example
	//serial.setup("/dev/ttyUSB0", 9600); //linux example

}

//--------------------------------------------------------------
void ardHandler::update()
{

}

void ardHandler::sendData(char msg)
{
    serial.writeByte(msg);
}