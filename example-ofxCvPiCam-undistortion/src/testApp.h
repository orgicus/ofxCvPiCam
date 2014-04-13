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
    cv::Mat frame,undistorted;
    ofxCv::Calibration calibration;
};
