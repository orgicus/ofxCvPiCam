#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
#include "ofxGui.h"
class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
        int thresh;
        ofxCvPiCam cam;

        ofxPanel gui,guiXtra;
        ofParameter<int> saturation;
        ofParameter<int> sharpness;
        ofParameter<int> contrast;
        ofParameter<int> brightness;
        ofParameter<int>  ISO;
        ofParameter<bool> vstabilisation;
        ofParameter<int> exposureMeteringMode;
        ofParameter<int>  exposureCompensation;
        ofParameter<int> exposureMode;
        ofParameter<int> shutterSpeed;
        ofParameter<int> rotation;
        ofParameter<bool> hflip;
        ofParameter<bool> vflip;
        ofParameter<float> roiX;
        ofParameter<float> roiY;
        ofParameter<float> roiW;
        ofParameter<float> roiH;
        ofParameter<int> awbMode;
        ofParameter<float> awbGainR;
        ofParameter<float> awbGainB;
        ofParameter<int> imageFX;
        ofRectangle ROI;
        string exposureModes[14];
        string exposureMeteringModes[5];
        string awbModes[11];
        string imageFXLabels[24];

        void saturationChanged(int &value);
        void sharpnessChanged(int &value);
        void contrastChanged(int &value);
        void brightnessChanged(int &value);
        void ISOChanged(int &value);
        void vstabilisationChanged(bool &value);
        void exposureCompensationChanged(int &value);
        void shutterSpeedChanged(int &value);
        void rotationChanged(int &value);
        void hflipChanged(bool &value);
        void vflipChanged(bool &value);
        void roiXChanged(float &value);
        void roiYChanged(float &value);
        void roiWChanged(float &value);
        void roiHChanged(float &value);
        void exposureMeteringModeChanged(int &value);
        void exposureModeChanged(int &value);
        void awbModeChanged(int &value);
        void awbGainRChanged(float &value);
        void awbGainBChanged(float &value);
        void imageFXChanged(int &value);

};

