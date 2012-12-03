
#include "ofMain.h"
#include "App.h"
#include "Def.h"
#include "ofAppGlutWindow.h"

void main(int argc, char* argv[])
{
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, SCREEN_WIDTH, SCREEN_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	App* app =  new App();
	ofSetFrameRate(60);
	ofRunApp(app);
	delete app;
}

/* EOF */