#include "ofMain.h"
#include "ofApp.h"
 
//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setGLVersion(4,1);
    settings.setSize(1280, 1024);
    // settings.windowMode = OF_FULLSCREEN;

    ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
