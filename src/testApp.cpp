#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(50,50,50);
	ofSetFrameRate(60);
    //ofSetVerticalSync(true);
	
	rgbaFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    sideCam = new camHandler(320, 240);
    sideCam->initCam(3); // DV intergrated cam
    
    topCam = new camHandler(320, 240);
    topCam->initCam(2); // iphone camera thingy
    
    // 3D stuff
    // this uses depth information for occlusion
	// rather than always drawing things on top of each other
	//glEnable(GL_DEPTH_TEST);
	
	// ofBox uses texture coordinates from 0-1, so you can load whatever
	// sized images you want and still use them to texture your box
	// but we have to explicitly normalize our tex coords here
	ofEnableNormalizedTexCoords();
    
    ofSetLineWidth(2);
    
    rgbaFbo.begin();
	ofClear(255,255,255, 0);
    rgbaFbo.end();
    
    arduino.setup(5);

}

//--------------------------------------------------------------
void testApp::update(){
    sideCam->update();
    topCam->update();
    
    ofEnableAlphaBlending();
	
    rgbaFbo.begin();
    draw3d();
    rgbaFbo.end();
    
    arduino.update(); // nothing happening yet..
}

//--------------------------------------------------------------
void testApp::draw3d(){
    
    ofClear(255,255,255, 0);
    
    
    // 3D stuff
	cam.begin();

    ofPushMatrix();
    ofTranslate(0,0,0);
    ofNoFill();
    ofSetColor(255,255,255);
    ofBox(300);
    ofSetColor(200, 100, 0);
    ofSphere(ofMap(topCam->blobX, 0, 320, -150, 150), ofMap(sideCam->blobX, 0, 320, -150, 150), ofMap(topCam->blobY, 0, 240, -150, 150), 5);
    

    ofSetColor(255, 255, 255);
    ofLine(ofMap(topCam->blobX, 0, 320, -150, 150), ofMap(sideCam->blobX, 0, 320, -150, 150), ofMap(topCam->blobY, 0, 240, -150, 150), -150, -150, -150);
    ofLine(ofMap(topCam->blobX, 0, 320, -150, 150), ofMap(sideCam->blobX, 0, 320, -150, 150), ofMap(topCam->blobY, 0, 240, -150, 150),  150,  150, -150);
    ofLine(ofMap(topCam->blobX, 0, 320, -150, 150), ofMap(sideCam->blobX, 0, 320, -150, 150), ofMap(topCam->blobY, 0, 240, -150, 150),  150, -150, -150);
    ofLine(ofMap(topCam->blobX, 0, 320, -150, 150), ofMap(sideCam->blobX, 0, 320, -150, 150), ofMap(topCam->blobY, 0, 240, -150, 150), -150,  150, -150);

    
    
    ofPopMatrix();
	
	cam.end();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofBackground(50, 50, 50);
    ofSetColor(255, 255, 255);
        
    // sidecam
    sideCam->drawOriginal(20, 20);
    sideCam->drawHsv(350, 20);
    sideCam->drawGrayscale(680, 20, true);
    
    // topcam
    topCam->drawOriginal(20, 300);
    topCam->drawHsv(350, 300);
    topCam->drawGrayscale(680, 300, true);
    
    ofSetColor(255,255,255);
    rgbaFbo.draw(0,0);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case OF_KEY_LEFT:
            arduino.sendData('L');
            break;
        case OF_KEY_RIGHT:
            arduino.sendData('R');
            break;
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    sideCam->changeTrackingColor(x, y, button);
    topCam->changeTrackingColor(x, y, button);
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
