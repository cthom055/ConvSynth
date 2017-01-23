#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    unsigned int buffersize = 1024;
    int samplerate = 44100;

    if (sample1.load (ofToDataPath ("mulchA1.wav"), 0)) {
        cout << sample1.getSummary();
        sample1.normalise();
    }

    if (sample2.load (ofToDataPath ("mulchB1.wav"), 0)) {
        cout << sample2.getSummary();
        sample2.normalise();
    }

    if (sample3.load (ofToDataPath ("stretchA.wav"), 0)) {
        cout << sample3.getSummary();
        sample3.normalise();
    }

    if (sample4.load (ofToDataPath ("pinknoise.wav"), 0)) {
        cout << sample4.getSummary();
        sample4.normalise();
    }

    //set pointers
    A = &sample1;
    B = &sample2;
    //init slider vals
    mag_slider = 0.5;
    phase_slider = 0.5;
    sA = 0;
    sB = 1;
    speed[0] = 1.0;
    speed[1] = 1.0;
    lock = true;

	//soundstream+fft
    soundstream.setup (2, 0, samplerate, buffersize, 4);
    fft.setup (1024, 1024, 128);
    fft2.setup (1024, 1024, 128);
    ifft.setup (1024, 1024, 128);

    for (int i = 0; i < fft.bins; i++) {
        mag1[i] = mag2[i] = phase1[i] = phase2[i] = 0;
    }

}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (imGui()) {
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased (int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved (int x, int y ) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged (int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed (int x, int y, int button) {
}

//--------------------------------------------------------------
bool ofApp::imGui()
{
    auto mainSettings = ofxImGui::Settings();
    this->gui.begin();
    ImGui::SetNextWindowSize (ImVec2 (500, 400));
    ImGui::Begin ("Convolution ##1");
    ImGui::Separator();

    //Draw Sliders
    ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (200, 10));
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (0.25f, 0.5f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_FrameBgHovered, ImColor::HSV ( 0.25f, 0.6f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_FrameBgActive, ImColor::HSV ( 0.25f, 0.7f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_SliderGrab, ImColor::HSV ( 0.25f, 0.9f, 0.9f));
    ImGui::Text ("Select Sample:");
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (0.f, 0.5f, 0.5f));

	//SAMPLE SELECTORS///
    if (ImGui::RadioButton ("1 ##A", &sA, 0)) { A = &sample1; };
    ImGui::SameLine();
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (.5f, 0.6f, 0.5f));
    if (ImGui::RadioButton ("1 ##B", &sB, 0)) { B = &sample1; };
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (0.f, 0.5f, 0.5f));
    if (ImGui::RadioButton ("2 ##A", &sA, 1)) { A = &sample2; };
    ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor::HSV(.5f, 0.6f, 0.5f));
    if (ImGui::RadioButton ("2 ##B", &sB, 1)) { B = &sample2; };
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (0.f, 0.5f, 0.5f));
    if (ImGui::RadioButton ("3 ##A", &sA, 2)) { A = &sample3; };
    ImGui::SameLine();
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (.5f, 0.6f, 0.5f));
    if (ImGui::RadioButton ("3 ##B", &sB, 2)) { B = &sample3; };
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (0.f, 0.5f, 0.5f));
    if (ImGui::RadioButton ("4 ##A", &sA, 3)) { A = &sample4; };
    ImGui::SameLine();
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (.5f, 0.6f, 0.5f));
    if (ImGui::RadioButton ("4 ##B", &sB, 3)) { B = &sample4; };
    ImGui::PopStyleColor (8);
    ImGui::Separator();
    ImGui::DragFloat2 ("Speed", speed, .05f, -3.0f, 3.0f, "%.2f", .5f);

    //magslider + colours
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (mag_slider / 2.f, 0.5f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_FrameBgHovered, ImColor::HSV (mag_slider / 2.f, 0.6f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_FrameBgActive, ImColor::HSV (mag_slider / 2.f, 0.7f, 0.5f));
	ImGui::DragFloat("Mag Mix", &mag_slider, 0.01, 0, 1);
    ImGui::PopStyleColor (3);

    //phaseslider + colours
    ImGui::PushStyleColor (ImGuiCol_FrameBg, ImColor::HSV (phase_slider / 2.f, 0.5f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_FrameBgHovered, ImColor::HSV (phase_slider / 2.f, 0.6f, 0.5f));
    ImGui::PushStyleColor (ImGuiCol_FrameBgActive, ImColor::HSV (phase_slider / 2.f, 0.7f, 0.5f));

    if (lock)	{ ImGui::DragFloat ("Phase Mix", &mag_slider, 0.01, 0, 1); phase_slider = mag_slider; }
    else		{ ImGui::DragFloat ("Phase Mix", &phase_slider, 0.01, 0, 1); }

    ImGui::PopStyleColor (3);
    ImGui::Checkbox ("lock mag/phase", &lock);
    ImGui::PopStyleColor (4);
    ImGui::PopStyleVar();
    ImGui::End();
    this->gui.end();
    return ImGui::IsMouseHoveringAnyWindow();
}

//--------------------------------------------------------------
void ofApp::audioOut (float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
    //at start of loop sets pointers to match those of A&B in the app/gui scope, if we changed them halfway through the audio callback this might break...
    maxiSample *sampleA, *sampleB;
    sampleA = A;		//A&B are declared in offApp.h scope
    sampleB = B;
    int chanA = sampleA->myChannels;
    int chanB = sampleB->myChannels;

    for (int i = 0; i < bufferSize; i++) {
        if (fft.process (sampleA->play (speed[0]) * 0.5)) {
            for (int j = 0; j < fft.bins; j++) {
                mag1[j] = fft.magnitudes[j];	//normalise by bin size
                phase1[j] = fft.phases[j];
            }
        }

        if (fft2.process (sampleB->play (speed[1]) * 0.5)) {
            for (int j = 0; j < fft2.bins; j++) {
                mag2[j] = fft2.magnitudes[j];	//normalise..
                phase2[j] = fft2.phases[j];
                float terpslide = (0.5 - abs (0.5 - mag_slider)); //ramps up 0 - 0.5 then down 0.5 - 1.0
                float maginterp1 = (mag1[j] * mag2[j] * terpslide);				//convolves, level - i.e when mag_slider is in middle will be loudest
                float magadd = (mag1[j] + ((mag2[j] - mag1[j]) * (mag_slider)));		//to allow for pure sample each side of slider
                float phaseinterp1 = (phase1[j] + ((phase2[j] - phase1[j]) * phase_slider)); //phases are added linearly
                m[j] =  (maginterp1 * 0.1) + (magadd * (0.5 - terpslide));
                p[j] = phaseinterp1;// *phaseinterp2;
            }
        }

        //fft2.process(sample2.playLoop(0, 1) * 0.8);
        myoutput[1] = myoutput[0] = ifft.process (m, p);
        //myoutput[1] = sample2.playLoop(0, 1) * 0.8;
        output[i * nChannels] = myoutput[0] * 0.9;
        output[i * nChannels + 1] = myoutput[1] * 0.9;
    }
}

void ofApp::exit() {
    soundstream.close();
}