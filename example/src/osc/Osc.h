/*!
 *	@����Ƃ�
 *		send����Α����܂��B
 *		�t�H�[�}�b�g������͈ȉ����g�p�ł��B
 *		i: int, f: float, l: long, s: string
 *
 *	@�󂯎��Ƃ�
 *		addReceiver�ɃA�h���X���w�肵�ēo�^���Ă��������B
 *		�w�肵���A�h���X�Ƀ��b�Z�[�W���󂯎��ƁA�o�^�����֐����Ă΂�܂��B
 *		�i�������ȍ~��ofAddListener���Q�l�ɂ��Ă��������j
 */

#pragma once

#include "ofTypes.h"
#include "ofEvents.h"
#include "ofxOscMessage.h"
#include <map>

namespace osc {
	// Sender�p
	void setupSender(const string& hostname, int port);
	void send(const string& address, const char* fmt, ...);
	void send(const string& address, const vector<int>& arg);
	void send(const string& address, const vector<float>& arg);
	void send(const string& address, const vector<string>& arg);
	void send(const string& address, int num, const int* arg);
	void send(const string& address, int num, const float* arg);

	// Receiver�p
	void setupReceiver(int port);
	template<class T>
	void addReceiveEvent(string address, T* obj, void (T::*method)(ofxOscMessage& msg));
	template<class T>
	void removeReceiveEvent(string address, T* obj, void (T::*method)(ofxOscMessage& msg));
	// �ȉ������Ȃ��ł����ł�
	extern map<string, ofEvent<ofxOscMessage>> events_;
	void receive();
}

template<class T>
void osc::addReceiveEvent(string address, T* obj, void (T::*method)(ofxOscMessage& msg))
{
	events_[address] += Poco::delegate(obj, method);
}

template<class T>
void osc::removeReceiveEvent(string address, T* obj, void (T::*method)(ofxOscMessage& msg))
{
	events_[address] -= Poco::delegate(obj, method);
}


/* EOF */
