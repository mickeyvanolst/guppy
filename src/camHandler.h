//
//  camHandler.h
//  lebensRaum
//
//  Created by Mick van Olst on 11/8/12.
//  Copyright (c) 2012 POPSURE. All rights reserved.
//

#ifndef lebensRaum_camHandler_h
#define lebensRaum_camHandler_h

#include "ofMain.h"
#include "ofxOpenCv.h"

class camHandler : ofBaseApp {
    
public:
    camHandler(int cWidth, int cHeight);
    
    void initCam(int cNumber);
    void update();
    
    void drawOriginal(int x, int y);
    void drawHsv(int x, int y);
    void drawGrayscale(int x, int y, bool seeBlobs);
    void changeTrackingColor(int x, int y, int button);
    
    float hue, sat;
    ofColor one;
    ofVec2f pos;
    
    ofVideoGrabber 		vidGrabber;         //our video grabber
    int                 camWidth;           //cam width
    int                 camHeight;          //cam height (duh)
    
    
    ofxCvColorImage		    colorImg;
    //Main color image which is gonna get wrapped
    
    ofxCvColorImage		    colorImgHSV;
    //the image doing the wrapping
    
    ofxCvGrayscaleImage		hueImg;     //Hue map
    ofxCvGrayscaleImage		satImg;     //Saturation map
    ofxCvGrayscaleImage     briImg;     //Brightness map
    
    ofxCvGrayscaleImage     reds;
    //Grayscale image we are gonna run the contour finder over to find our color
    
    unsigned char *         colorTrackedPixelsRed;
    //just some raw images which we are gonna put pixels into
    
    ofTexture               trackedTextureRed;
    //color texture that we are gonna draw to
    
    ofxCvContourFinder      finderRed;
    //contour finder, very handy
    
    int                 posCorX;
    int                 posCorY;
    
    int                 camID;
    
    float               blobX;
    float               blobY;
    
private:
    
};
    
#endif
