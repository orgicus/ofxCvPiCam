#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
    int w,h;
    ofxCvPiCam cam;
    cv::Mat frame;
    ofImage frameOF;
	ofxCv::FlowFarneback flow;
	ofMesh mesh;
	int stepSize, xSteps, ySteps;
};
