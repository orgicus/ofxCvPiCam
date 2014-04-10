#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
    //ofVideoGrabber cam;
    ofxCvPiCam cam;
	ofxCv::ObjectFinder finder;
};
