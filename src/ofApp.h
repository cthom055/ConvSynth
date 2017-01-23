#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		bool imGui();			//gui
		void audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
		void exit();

		//GUI
		ofxImGui::Gui	gui;

		ofSoundStream	soundstream;
		maxiOsc			osc1, osc2;
		maxiFFT			fft, fft2;
		maxiIFFT		ifft, ifft2;
		bool			lock;
		int				sA, sB;
		float			phase_slider, mag_slider, fade;
		float			speed[2];
		float			myoutput[2];
		float			mag1[512], phase1[512], mag2[512], phase2[512], m[512], p[512];
		maxiSample		sample1, sample2, sample3, sample4, *A, *B;

};
