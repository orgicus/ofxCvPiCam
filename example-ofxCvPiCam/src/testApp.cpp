#include "testApp.h"
#include <opencv2/opencv.hpp>

using namespace ofxCv;
using namespace cv;

Mat frame,frameProcessed;

void testApp::onCharacterReceived(SSHKeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
//--------------------------------------------------------------
void testApp::setup(){
    consoleListener.setup(this);
    consoleListener.startThread(false, false);

    cam.setup(320,240,false);//setup camera (w,h,color = true,gray = false);
    thresh = 127;
}

//--------------------------------------------------------------
void testApp::update(){
    frame = cam.grab();
}

//--------------------------------------------------------------
void testApp::draw(){
    if(!frame.empty()) {
        threshold(frame,frameProcessed,thresh,255,0);
        drawMat(frame,0,0);
        drawMat(frameProcessed,320,0);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    ofLogVerbose() << "keyPressed: " << key;
    if(key == 't' && thresh > 0)   thresh--;
    if(key == 'T' && thresh < 255) thresh++;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    thresh = ofMap(x,0,ofGetWidth(),0,255);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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
