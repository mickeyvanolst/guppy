//
//  camHandler.cpp
//  lebensRaum
//
//  Created by Mick van Olst on 11/8/12.
//  Copyright (c) 2012 POPSURE. All rights reserved.
//
//

#include "camHandler.h"

//--------------------------------------------------------------
camHandler::camHandler(int cWidth, int cHeight)
{
    camWidth 		= cWidth;	// try to grab at this size.
    camHeight 		= cHeight;
    
    pos.set(0, 0);
    
    blobX = 0;
    blobY = 0;
    
    colorImg.allocate(camWidth,camHeight);
    //Image that will house the cameras output, used because of laziness
    
    colorImgHSV.allocate(camWidth,camHeight);
    //our HSB image that will house the color image and deal out the Hue, Saturation and brightness
    
    hueImg.allocate(camWidth,camHeight);        //Hue map
    satImg.allocate(camWidth,camHeight);        //saturation map
    briImg.allocate(camWidth,camHeight);        //brightness map, not gonna be used but necessary
    
    reds.allocate(camWidth, camHeight);         //our postRange image basically
    
    colorTrackedPixelsRed = new unsigned char [camWidth * camHeight];     //rangeImage
    
    trackedTextureRed.allocate(camWidth, camHeight, GL_LUMINANCE);        //final output

}

//--------------------------------------------------------------
void camHandler::initCam(int cNumber)
{
    vidGrabber.listDevices();
    
    vidGrabber.setDeviceID(cNumber);
    vidGrabber.setVerbose(true);                    //just some text for debugging
    vidGrabber.initGrabber(camWidth,camHeight);     //start the show!
    
    camID = cNumber;

}

//--------------------------------------------------------------
void camHandler::update()
{
    vidGrabber.grabFrame();
    
    colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
    
    colorImgHSV = colorImg;
    colorImgHSV.convertRgbToHsv();
    
    colorImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, briImg);
    
    hueImg.flagImageChanged();
    satImg.flagImageChanged();
    briImg.flagImageChanged();
    
    unsigned char * huePixels = hueImg.getPixels();
    unsigned char * satPixels = satImg.getPixels();
    
    int nPixels = camWidth * camHeight;
    
    // going trough them pixels..
    
    for (int i = 0; i < nPixels; i++) {
        if((huePixels[i] >= hue - 12 && huePixels[i] <= hue + 12) &&
           (satPixels[i] >= sat -24 && satPixels[i] <= sat + 200)) {
            colorTrackedPixelsRed[i] = 255;
        } else {
            colorTrackedPixelsRed[i] = 0;
        }
    }
    
    reds.setFromPixels(colorTrackedPixelsRed, camWidth, camHeight);
    finderRed.findContours(reds, 10, nPixels/3, 1, false);
    
    trackedTextureRed.loadData(colorTrackedPixelsRed, camWidth, camHeight, GL_LUMINANCE);
    
    //--------
    
    if(finderRed.blobs.size() > 0) {
        pos = ofVec2f(finderRed.blobs[0].centroid.x, finderRed.blobs[0].centroid.y);
        
        blobX = finderRed.blobs[0].centroid.x;
        blobY = finderRed.blobs[0].centroid.y;
    }

}

//--------------------------------------------------------------
void camHandler::drawOriginal(int x, int y)
{
    vidGrabber.draw(x, y);
    
    posCorX = x;
    posCorY = y;
}

//--------------------------------------------------------------
void camHandler::drawHsv(int x, int y)
{
    colorImgHSV.draw(x, y);
}

//--------------------------------------------------------------
void camHandler::drawGrayscale(int x, int y, bool seeBlobs)
{
    trackedTextureRed.draw(x, y);
    if(seeBlobs) {
        finderRed.draw(x,y);

        glPushMatrix();
        glTranslatef(x, y, 0);
        finderRed.draw();
        glPopMatrix();
        
        if(finderRed.blobs.size() > 0) {
            char tempStr1[255];
            sprintf(tempStr1, "x : %f\ny : %f", finderRed.blobs[0].centroid.x, finderRed.blobs[0].centroid.y);
            ofDrawBitmapString(tempStr1, x, y + camHeight + 15);
        }
    }
}

//--------------------------------------------------------------
void camHandler::changeTrackingColor(int x, int y, int button){
    
    if(x >= posCorX && x <= (posCorX + camWidth) && y >= posCorY && y <= (posCorY + camHeight)) {
        
        unsigned char * huePixels = hueImg.getPixels();
        unsigned char * satPixels = satImg.getPixels();
        
        x = MIN(x-posCorX,hueImg.width-1);
        y = MIN(y-posCorY, hueImg.height-1);
        
        if(button == 0) {
            hue = huePixels[x+(y*hueImg.width)]; // set the hue
            sat = satPixels[x+(y*satImg.width)]; // set the sat
        }
    }
}