#include "testApp.h"

using namespace ofxCv;
using namespace cv;
Mat frame;
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	finder.setup("haarcascade_frontalface_alt2.xml");
	finder.setPreset(ObjectFinder::Fast);
    //cam.initGrabber(640, 480);
    cam.setup(320,240,false);
}

void testApp::update() {
    /*
	cam.update();
	if(cam.isFrameNew()) {
		finder.update(cam);
	}
    */
    frame = cam.grab();
    if(!frame.empty()) finder.updateGray(frame);
}

void testApp::draw() {
    if(!frame.empty()){
        drawMat(frame,0,0);
        finder.draw();
    }
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}
