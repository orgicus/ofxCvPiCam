#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
    ofxCvPiCam cam;
    cv::Mat frame;
    ofxCv::ContourFinder contourFinder;
	float threshold;
	ofxCv::TrackingColorMode trackingColorMode;
	ofColor targetColor;
};
