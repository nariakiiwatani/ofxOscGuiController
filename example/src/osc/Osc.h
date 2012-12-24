/*!
 *	@送るとき
 *		sendすれば送られます。
 *		フォーマット文字列は以下が使用可です。
 *		i: int, f: float, l: long, s: string
 *
 *	@受け取るとき
 *		addReceiverにアドレスを指定して登録してください。
 *		指定したアドレスにメッセージを受け取ると、登録した関数が呼ばれます。
 *		（第二引数以降はofAddListenerを参考にしてください）
 */

#pragma once

#include "ofTypes.h"
#include "ofEvents.h"
#include "ofxOscMessage.h"
#include <map>

namespace osc {
	// Sender用
	void setupSender(const string& hostname, int port);
	void send(const string& address, const char* fmt, ...);
	void send(const string& address, const vector<int>& arg);
	void send(const string& address, const vector<float>& arg);
	void send(const string& address, const vector<string>& arg);
	void send(const string& address, int num, const int* arg);
	void send(const string& address, int num, const float* arg);

	// Receiver用
	void setupReceiver(int port);
	template<class T>
	void addReceiveEvent(string address, T* obj, void (T::*method)(ofxOscMessage& msg));
	template<class T>
	void removeReceiveEvent(string address, T* obj, void (T::*method)(ofxOscMessage& msg));
	// 以下さわんないでいいです
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
