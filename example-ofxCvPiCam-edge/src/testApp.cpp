#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    cam.setup(320, 240,false);
}

void testApp::update() {
    frame = cam.grab();
    if(!frame.empty()) {
        Canny(frame, edge, mouseX, mouseY, 3);
	}
}

void testApp::draw() {
    if(!frame.empty()){
        drawMat(frame,0, 0);
        drawMat(edge,320, 0);
    }
}
