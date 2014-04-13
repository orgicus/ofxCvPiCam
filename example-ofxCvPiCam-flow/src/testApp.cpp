#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    w = 160;h = 120;
    cam.setup(w, h,false);

    panel.setPosition(250, 800);
	
    panel.setup("Optical Flow");
	
    panel.add(pyrScale.set("pyrScale", .5, 0, 1));
    panel.add(levels.set("levels", 4, 1, 8));
    panel.add(winsize.set("winsize", 8, 4, 64));
    panel.add(iterations.set("iterations", 2, 1, 8));
    panel.add(polyN.set("polyN", 7, 5, 10));
    panel.add(polySigma.set("polySigma", 1.5, 1.1, 2));
    panel.add(OPTFLOW_FARNEBACK_GAUSSIAN.set("OPTFLOW_FARNEBACK_GAUSSIAN", false));
	
    panel.add(useFarneback.set("useFarneback", true));
    panel.add(winSize.set("winSize", 32, 4, 64));
    panel.add(maxLevel.set("maxLevel", 3, 0, 8));
	
    panel.add(maxFeatures.set("maxFeatures", 200, 1, 1000));
    panel.add(qualityLevel.set("qualityLevel", 0.01, 0.001, .02));
    panel.add(minDistance.set("minDistance", 4, 1, 16));

    curFlow = &farneback;
}

void testApp::update(){
    frame = cam.grab();
    if(!frame.empty()) {

        if(useFarneback.get()) {
			curFlow = &farneback;
            farneback.setPyramidScale( pyrScale.get() );
            farneback.setNumLevels( levels.get() );
            farneback.setWindowSize( winsize.get() );
            farneback.setNumIterations( iterations.get() );
            farneback.setPolyN( polyN.get() );
            farneback.setPolySigma( polySigma.get() );
            farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN.get());
			
		} else {
			curFlow = &pyrLk;
            pyrLk.setMaxFeatures( maxFeatures.get() );
            pyrLk.setQualityLevel( qualityLevel.get() );
            pyrLk.setMinDistance( minDistance.get() );
            pyrLk.setWindowSize( winSize.get() );
            pyrLk.setMaxLevel( maxLevel.get() );
		}
        //check it out that that you can use Flow polymorphically
        curFlow->calcOpticalFlow(frame);
	}
}

void testApp::draw(){
	ofBackground(0);
		
	ofSetColor(255);
    if(!frame.empty()) drawMat(frame,220,0,w*4,h*4);
    curFlow->draw(220,0,w*4,h*4);
    panel.draw();
}
