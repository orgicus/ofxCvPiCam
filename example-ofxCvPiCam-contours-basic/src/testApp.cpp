#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    cam.setup(640, 480,false);
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
}

void testApp::update() {
    frame = cam.grab();
    if(!frame.empty()) {
		contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));
        contourFinder.findContours(frame);
	}
}

void testApp::draw() {
	ofSetColor(255);
    if(!frame.empty()) drawMat(frame,0, 0);
	contourFinder.draw();
}
