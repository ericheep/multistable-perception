#include "ofMain.h"
#include "ofApp.h"
 
//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setGLVersion(4,1);
    //settings.setSize(600, 400);
    
    // enable for fullscreen on second screen
    settings.setSize(1920, 1080);
    settings.setPosition(glm::vec2(1920,0));
    settings.windowMode = OF_FULLSCREEN;

    ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
