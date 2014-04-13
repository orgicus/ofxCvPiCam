#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
    w = 160;h = 120;
    cam.setup(w, h,false);
    frameOF.allocate(w,h,OF_IMAGE_GRAYSCALE);
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	stepSize = 8;
    ySteps = h / stepSize;
    xSteps = w / stepSize;
	for(int y = 0; y < ySteps; y++) {
		for(int x = 0; x < xSteps; x++) {
			mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
			mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
		}
	}
	for(int y = 0; y + 1 < ySteps; y++) {
		for(int x = 0; x + 1 < xSteps; x++) {
			int nw = y * xSteps + x;
			int ne = nw + 1;
			int sw = nw + xSteps;
			int se = sw + 1;
			mesh.addIndex(nw);
			mesh.addIndex(ne);
			mesh.addIndex(se);
			mesh.addIndex(nw);
			mesh.addIndex(se);
			mesh.addIndex(sw);
		}
	}
}

void testApp::update() {
    frame = cam.grab();
    if(!frame.empty()) {
        toOf(frame,frameOF);frameOF.update();
		flow.setWindowSize(8);
        flow.calcOpticalFlow(frame);
        int i = 0;
        float distortionStrength = 4;
		for(int y = 1; y + 1 < ySteps; y++) {
			for(int x = 1; x + 1 < xSteps; x++) {
                i = y * xSteps + x;
				ofVec2f position(x * stepSize, y * stepSize);
				ofRectangle area(position - ofVec2f(stepSize, stepSize) / 2, stepSize, stepSize);
				ofVec2f offset = flow.getAverageFlowInRegion(area);
				mesh.setVertex(i, position + distortionStrength * offset);
				i++;
			}
		}
	}
}

void testApp::draw() {
	ofBackground(0);
    ofScale(4, 4);
    frameOF.getTextureReference().bind();
	mesh.draw();
    frameOF.getTextureReference().unbind();
	if(ofGetMousePressed()) {
		mesh.drawWireframe();
	}
}
