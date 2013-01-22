#pragma once

#include "ofxOscSender.h"
#include "ofxParamPanel.h"
#include "ofxXmlSettings.h"

class ofxOscGuiPanel;
class ofxOscGuiController
{
public:
	ofxOscGuiController(string name);
	~ofxOscGuiController();

	void loadSettingFromFile();
	void saveSettingToFile();
	void loadSettingFromXml(ofxXmlSettings& xml);
	void saveSettingToXml(ofxXmlSettings& xml);

	void loadParamFromFile();
	void saveParamToFile();

	void draw();

private:
	string name_;
	ofxOscSender osc_sender_;

	ofxParamPanel panel_;
	vector<ofxOscGuiPanel*> osc_panel_;

	string host_;
	int port_;
};

class ofxOscGuiPanel : public ofxParamPanel
{
public:
	ofxOscGuiPanel(ofxOscSender* osc_sender, string address, string folder);
	ofxOscGuiPanel(ofxOscSender* osc_sender, string address, string folder, ofxXmlSettings& xml);
	~ofxOscGuiPanel();
	void send();

	void loadSettingFromXml(ofxXmlSettings& xml);
	void saveSettingToXml(ofxXmlSettings& xml);

	const string& getAddress() { return address_; }

private:
	void addDefault();
	void onSendButton(bool&);
	void onChange(int&);
	void onChange(float&);
	void onChange(bool&);
private:
	ofxOscSender* osc_sender_;
	bool send_always_;
	enum Type {
		S32,
		F32,
		BOOL,
		STRING,
	};
	struct Value {
		Type type;
		string name;
		union {
			int i;
			float f;
			bool b;
		} val, min, max;
		string s;
	};
	vector<Value> value_;

	string address_;
};

/* EOF */
