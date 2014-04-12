#include "testApp.h"

using namespace ofxCv;
using namespace cv;
Mat frame;
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	finder.setup("haarcascade_frontalface_alt2.xml");
	finder.setPreset(ObjectFinder::Fast);
    cam.setup(320,240,true);
}

void testApp::update() {
    frame = cam.grab();
    if(!frame.empty()) finder.update(frame);
}

void testApp::draw() {
    if(!frame.empty()){
        drawMat(frame,0,0);
        finder.draw();
    }
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}
