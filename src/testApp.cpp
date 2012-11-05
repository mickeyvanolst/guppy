#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(50,50,50);
	ofSetFrameRate(60);
    //ofSetVerticalSync(true);
	
	rgbaFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    sideCam = new camHandler(320, 240);
    sideCam->initCam(4); // DV intergrated cam
    
    topCam = new camHandler(320, 240);
    topCam->initCam(2); // iphone camera thingy
    
    // 3D stuff
    // this uses depth information for occlusion
	// rather than always drawing things on top of each other
	//glEnable(GL_DEPTH_TEST);
    
	
	// ofBox uses texture coordinates fro 0-1, so you can load whatever
	// sized images you want and still use them to texture your box
	// but we have to explicitly normalize our tex coords here
	//ofEnableNormalizedTexCoords();
    
    ofSetLineWidth(2);
    
    rgbaFbo.begin();
	ofClear(255,255,255, 0);
    rgbaFbo.end();
    
    arduino.setup(5);
    
    toggleCamview = true;
    toggleArdSend = false;
    
    radius = 5;
    
    motorX = 0;
    motorY = 0;
    motorZ = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    sideCam->update();
    topCam->update();
    
    ofEnableAlphaBlending();
	
    rgbaFbo.begin();
    draw3d();
    rgbaFbo.end();
    
    
    Tweener.update();
    Tweener.addTween(position.x, ofMap(sideCam->blobX, 0, 320, -125, 125), 2); // aquarium case x
    Tweener.addTween(position.y, ofMap(sideCam->blobY, 0, 240, -125, 125), 2); // aquarium case y
    Tweener.addTween(position.z, ofMap(topCam->blobY, 0, 240, -125, 125), 2); // aquarium case z
    
    if(toggleArdSend) {
        //if (position.x != motorX || position.y != motorY || position.z != motorZ) {
            if(arduino.ardReady) {
                printf("sending coordinates to arduino\n");
                arduino.writeString(ofToString(int(position.x)) + "," + ofToString(int(position.y)) + "," + ofToString(int(position.z)) + ",;");
            }
        //}
    }   
    
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
    
    sBox(0, 0, 0, 150, 150, 150); // outer case
    sBox(0, -144, position.z, 6, 150, 20); // top move bar
    sBox(position.x, position.y, position.z, 50, 50, 50); // aquarium case
    
    ofSetColor(200, 100, 0);
    ofSphere(ofMap(sideCam->blobX, 0, 320, -125, 125), ofMap(sideCam->blobY, 0, 240, -125, 125), ofMap(topCam->blobY, 0, 240, -125, 125), 5);
    
    ofPopMatrix();
	
	cam.end();
     
}

//--------------------------------------------------------------
void testApp::sBox(float x, float y, float z, float h, float l, float d){
    
    ofPushMatrix();
    ofTranslate(x, y, z);
    //top
    ofBeginShape();
    ofVertex(-l, h, -d);
    ofVertex(-l, h,  d);
    ofVertex( l, h,  d);
    ofVertex( l, h, -d);
    ofVertex(-l, h, -d);
    ofEndShape();
    //bottom
    ofBeginShape();
    ofVertex(-l, -h, -d);
    ofVertex(-l, -h,  d);
    ofVertex( l, -h,  d);
    ofVertex( l, -h, -d);
    ofVertex(-l, -h, -d);
    ofEndShape();
    //front
    ofBeginShape();
    ofVertex(-l, h, -d);
    ofVertex(l, h, -d);
    ofVertex(l, -h, -d);
    ofVertex(-l, -h, -d);
    ofVertex(-l, h, -d);
    ofEndShape();
    //back
    ofBeginShape();
    ofVertex(-l,  h,  d);
    ofVertex( l,  h,  d);
    ofVertex( l, -h,  d);
    ofVertex(-l, -h,  d);
    ofVertex(-l,  h,  d);
    ofEndShape();
    //left
    //right
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofBackground(50, 50, 50);
    ofSetColor(255, 255, 255);
    
    if(toggleCamview) {
        // sidecam
        sideCam->drawOriginal(20, 20);
        sideCam->drawHsv(350, 20);
        sideCam->drawGrayscale(680, 20, true);
        
        // topcam
        topCam->drawOriginal(20, 300);
        topCam->drawHsv(350, 300);
        topCam->drawGrayscale(680, 300, true);
    }
    
    ofSetColor(255,255,255);
    rgbaFbo.draw(0,0);
    

    //ofCircle(position.x, position.y, radius);
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
        case 'c':
            toggleCamview = !toggleCamview;
            break;
        case 's':
            toggleArdSend = !toggleArdSend;
            arduino.writeString("hi,there,friend,;");
            break;
        case '1':
            arduino.writeString("-125,-125,-125,;");
            break;
        case '2':
            arduino.writeString("125,125,125,;");
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
    
    //Tweener.addTween(position.x, ofRandom(1024), 5);
    //Tweener.addTween(position.y, ofRandom(768), 5);
    //arduino.writeString(ofToString(int(position.x)) + "," + ofToString(int(position.y)) + "," + ofToString(int(position.z)) + ",;");
    
    
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
