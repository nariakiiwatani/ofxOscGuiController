#pragma once

#include "ofxOscSender.h"
#include "ofxParamEdit.h"

class ofxOscGuiPanel;
class ofxOscGuiController
{
public:
	ofxOscGuiController(string name);
	~ofxOscGuiController();

	void loadFromFile();
	void saveToFile();

	void loadFromXml(ofxXmlSettings& xml);
	void saveToXml(ofxXmlSettings& xml);

	void draw();

private:
	string name_;
	ofxOscSender osc_sender_;
	vector<ofxOscGuiPanel*> panel_;

	string host_;
	int port_;
};

class ofxOscGuiPanel : public ofxParamEdit
{
public:
	ofxOscGuiPanel(ofxOscSender* osc_sender, string address, string folder);
	ofxOscGuiPanel(ofxOscSender* osc_sender, string address, string folder, ofxXmlSettings& xml);
	~ofxOscGuiPanel();
	void send();

	void loadFromXml(ofxXmlSettings& xml);
	void saveToXml(ofxXmlSettings& xml);

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
	};
	struct Value {
		Type type;
		string name;
		union {
			int i;
			float f;
			bool b;
		} val, min, max;
	};
	vector<Value> value_;

	string address_;
};

/* EOF */