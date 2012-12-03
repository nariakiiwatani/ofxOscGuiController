#include "Osc.h"
#include "ofxOscSender.h"
#include "ofxOscReceiver.h"

namespace osc {
ofxOscSender sender_;
ofxOscReceiver receiver_;
map<string, ofEvent<ofxOscMessage>> events_;
}
void osc::setupSender(const string& hostname, int port)
{
	sender_.setup(hostname, port);
}

void osc::setupReceiver(int port) {
	receiver_.setup(port);
}

void osc::send(const string& address, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	ofxOscMessage msg;
	msg.setAddress(address);
	while(*fmt) {
		switch(*fmt++) {
		case 'i': msg.addIntArg(va_arg(ap, int));		break;
		case 'f': msg.addFloatArg(va_arg(ap, double));	break;
		case 'l': msg.addInt64Arg(va_arg(ap, long));	break;
		case 's': msg.addStringArg(va_arg(ap, char*));	break;
		}
	}
	sender_.sendMessage(msg);
	va_end(ap);
}

void osc::send(const string& address, int num, const int* arg)
{
	ofxOscMessage msg;
	msg.setAddress(address);
	while(num--) {
		msg.addIntArg(*arg++);
	}
	sender_.sendMessage(msg);
}
void osc::send(const string& address, const vector<int>& arg)
{
	ofxOscMessage msg;
	msg.setAddress(address);
	vector<int>::const_iterator it = arg.begin();
	vector<int>::const_iterator end = arg.end();
	while(it != end) {
		msg.addIntArg(*it++);
	}
	sender_.sendMessage(msg);
}

void osc::send(const string& address, int num, const float* arg)
{
	ofxOscMessage msg;
	msg.setAddress(address);
	while(num--) {
		msg.addFloatArg(*arg++);
	}
	sender_.sendMessage(msg);
}
void osc::send(const string& address, const vector<float>& arg)
{
	ofxOscMessage msg;
	msg.setAddress(address);
	vector<float>::const_iterator it = arg.begin();
	vector<float>::const_iterator end = arg.end();
	while(it != end) {
		msg.addFloatArg(*it++);
	}
	sender_.sendMessage(msg);
}

void osc::receive() {
	if(receiver_.hasWaitingMessages()) {
		ofxOscMessage msg;
		while(receiver_.getNextMessage(&msg)) {
			map<string, ofEvent<ofxOscMessage>>::iterator it = events_.find(msg.getAddress());
			if(it != events_.end()) {
				(*it).second.notify(NULL, msg);
			}
		}
	}
}


/* EOF */
