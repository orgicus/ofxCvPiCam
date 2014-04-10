#include "testApp.h"

using namespace cv;
using namespace ofxCv;

void testApp::setup() {
    cam.setup(320, 240,false);
}

void testApp::update() {
    frame = cam.grab();
    thresh = frame.clone();
    if(!thresh.empty()){
        if(ofGetMousePressed()) {
            autothreshold(thresh);
        } else {
            float thresholdValue = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
            threshold(thresh, thresholdValue);
        }
    }
}

void testApp::draw() {
    if(!frame.empty()){
        drawMat(frame,0,0);
        drawMat(thresh,320, 0);
    }
}
