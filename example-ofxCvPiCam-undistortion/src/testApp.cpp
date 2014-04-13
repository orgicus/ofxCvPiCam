#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
    w = 320;h = 240;
    cam.setup(w, h,false);
	calibration.setFillFrame(true); // true by default
    calibration.load("PiCam.yml");
    bool hasFrame = false;
    while(!hasFrame){
        frame = cam.grab();
        if(!frame.empty()){
            imitate(undistorted, frame);
            hasFrame = true;
        }
    }
}

void testApp::update() {
    frame = cam.grab();
    if(!frame.empty()){
        calibration.undistort(frame, undistorted);
	}
}

void testApp::draw() {
	ofSetColor(255);
    if(!frame.empty()) drawMat(frame,0, 0);
                       drawMat(undistorted,w, 0);
}
