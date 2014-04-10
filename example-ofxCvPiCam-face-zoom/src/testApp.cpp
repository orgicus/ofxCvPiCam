#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	objectFinder.setup(ofToDataPath("haarcascade_frontalface_alt2.xml"));
	objectFinder.setPreset(ObjectFinder::Fast);
    cam.setup(640, 480,false);
    cropped.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
    frame = cam.grab();
    if(!frame.empty()) {
        objectFinder.updateGray(frame);
		if(objectFinder.size() > 0) {
			cv::Rect roi = toCv(objectFinder.getObject(0));
            Mat croppedCamMat(frame, roi);
            resize(croppedCamMat, cropped);
            cropped.update();
		}
	}
}

void testApp::draw() {
    cropped.draw(0, 0);
}
