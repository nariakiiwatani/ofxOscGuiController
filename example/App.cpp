/*!
*/

#include "App.h"
#include "ofxOscGuiController.h"
#include "osc/Osc.h"

namespace {
	ofxOscGuiController* cont_;
}
void App::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	cont_ = new ofxOscGuiController("hoge");
	osc::setupReceiver(7400);
}

void App::update()
{
	osc::receive();
}

void App::draw()
{
	cont_->draw();
}

void App::exit()
{
	cont_->saveToFile();
}

/* EOF */