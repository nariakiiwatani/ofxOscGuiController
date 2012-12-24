#include "ofxOscGuiController.h"

ofxOscGuiController::~ofxOscGuiController()
{
	while(!panel_.empty()) {
		delete panel_.at(0);
		panel_.erase(panel_.begin());
	}
}

void ofxOscGuiController::setName(const string& name)
{
	name_ = name;
}

void ofxOscGuiController::loadFromFile()
{
	ofxXmlSettings xml;
	xml.loadFile(name_+"/settings.xml");
	loadFromXml(xml);
}

void ofxOscGuiController::saveToFile()
{
	ofxXmlSettings xml;
	saveToXml(xml);
	xml.saveFile(name_+"/settings.xml");
}

void ofxOscGuiController::loadFromXml(ofxXmlSettings& xml)
{
	if(xml.pushTag("osc")) {
		host_ = xml.getValue("host", "localhost");
		port_ = xml.getValue("port", 7400);
		xml.popTag();
	}
	else {
		host_ = "localhost";
		port_ = 7400;
	}
	osc_sender_.setup(host_, port_);

	int panel_count = xml.getNumTags("panel");
	for(int i = 0; i < panel_count; ++i) {
		string name = xml.getAttribute("panel", "name", "", i);
		xml.pushTag("panel", i);
		ofxOscGuiPanel* panel = new ofxOscGuiPanel(&osc_sender_, name, name_, xml);
		panel_.push_back(panel);
		xml.popTag();
	}
}

void ofxOscGuiController::saveToXml(ofxXmlSettings& xml)
{
	xml.addTag("osc");
	xml.pushTag("osc");
	xml.addValue("host", host_);
	xml.addValue("port", port_);
	xml.popTag();

	vector<ofxOscGuiPanel*>::iterator it = panel_.begin();
	vector<ofxOscGuiPanel*>::iterator end = panel_.end();
	int index = 0;
	while(it != end) {
		xml.addTag("panel");
		xml.addAttribute("panel", "name", (*it)->getAddress(), index);
		xml.pushTag("panel", index);
		(*it++)->saveToXml(xml);
		xml.popTag();
		++index;
	}
}

void ofxOscGuiController::draw()
{
	vector<ofxOscGuiPanel*>::iterator it = panel_.begin();
	vector<ofxOscGuiPanel*>::iterator end = panel_.end();
	while(it != end) {
		(*it++)->draw();
	}
}

ofxOscGuiPanel::ofxOscGuiPanel(ofxOscSender* osc_sender, string address, string folder)
:ofxParamPanel()
,osc_sender_(osc_sender)
,send_always_(false)
,address_(address)
{
	setup(folder+"/"+address, folder+"/"+address+".xml");
	addDefault();
	load();
}

ofxOscGuiPanel::ofxOscGuiPanel(ofxOscSender* osc_sender, string address, string folder, ofxXmlSettings& xml)
:ofxParamPanel()
,osc_sender_(osc_sender)
,send_always_(false)
,address_(address)
{
	setup(folder+"/"+address, folder+"/"+address+".xml");
	addDefault();
	loadSettingFromXml(xml);
	load();
}

ofxOscGuiPanel::~ofxOscGuiPanel()
{
}

void ofxOscGuiPanel::addDefault()
{
	addButton("bang", this, &ofxOscGuiPanel::onSendButton);
	addToggle("send always", send_always_);
}

void ofxOscGuiPanel::loadSettingFromXml(ofxXmlSettings& xml)
{
	setPosition(xml.getValue("x", 0), xml.getValue("y", 0));
	int num = xml.getNumTags("control");
	value_.resize(num);
	for(int i = 0; i < num; ++i) {
		string type = xml.getAttribute("control", "type", "", i);
		xml.pushTag("control", i);
		if(type == "int") {
			value_[i].type = S32;
			value_[i].name = xml.getValue("name", "");
			value_[i].min.i = xml.getValue("min", 0);
			value_[i].max.i = xml.getValue("max", 255);
			addSlider(value_[i].name, value_[i].val.i, value_[i].min.i, value_[i].max.i, this, &ofxOscGuiPanel::onChange);
		}
		else if(type=="float") {
			value_[i].type = F32;
			value_[i].name = xml.getValue("name", "");
			value_[i].min.f = xml.getValue("min", -1);
			value_[i].max.f = xml.getValue("max", 1);
			addSlider(value_[i].name, value_[i].val.f, value_[i].min.f, value_[i].max.f, this, &ofxOscGuiPanel::onChange);
		}
		else if(type == "bool") {
			value_[i].type = BOOL;
			value_[i].name = xml.getValue("name", "");
			addToggle(value_[i].name, value_[i].val.b, this, &ofxOscGuiPanel::onChange);
		}
		xml.popTag();
	}
}

void ofxOscGuiPanel::saveSettingToXml(ofxXmlSettings& xml)
{
	const ofPoint& pos = getPosition();
	xml.addValue("x", pos.x);
	xml.addValue("y", pos.y);

	vector<Value>::iterator it = value_.begin();
	vector<Value>::iterator end = value_.end();
	int index = 0;
	while(it != end) {
		xml.addTag("control");
		switch((*it).type) {
		case S32:	xml.addAttribute("control", "type", "int", index);		break;
		case F32:	xml.addAttribute("control", "type", "float", index);	break;
		case BOOL:	xml.addAttribute("control", "type", "bool", index);		break;
		}
		xml.pushTag("control", index);
		xml.addValue("name", (*it).name);
		switch((*it).type) {
		case S32:
			xml.addValue("min", (*it).min.i);
			xml.addValue("max", (*it).max.i);
			break;
		case F32:
			xml.addValue("min", (*it).min.f);
			xml.addValue("max", (*it).max.f);
			break;
		case BOOL:
			break;
		}
		xml.popTag();
		++it;
		++index;
	}
}

void ofxOscGuiPanel::send()
{
	ofxOscMessage msg;
	msg.setAddress("/"+address_);
	vector<Value>::iterator it = value_.begin();
	vector<Value>::iterator end = value_.end();
	while(it != end) {
		switch((*it).type) {
		case S32:	msg.addIntArg((*it).val.i);		break;
		case F32:	msg.addFloatArg((*it).val.f);	break;
		case BOOL:	msg.addIntArg((*it).val.b?1:0);	break;
		}
		++it;
	}
	osc_sender_->sendMessage(msg);
}

void ofxOscGuiPanel::onSendButton(bool&)
{
	send();
}
void ofxOscGuiPanel::onChange(int&)
{
	if(send_always_) {
		send();
	}
}
void ofxOscGuiPanel::onChange(float&)
{
	if(send_always_) {
		send();
	}
}
void ofxOscGuiPanel::onChange(bool&)
{
	if(send_always_) {
		send();
	}
}

/* EOF */
