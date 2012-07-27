/*
 * eventtest.cpp
 *
 *  Created on: 27.07.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <Events.h>

using namespace std;

class TestSender {
	EventCaller<TestSender> _testEvent;
public:
	TestSender() {}
	virtual ~TestSender() {}

	virtual int test() {
		return 1;
	}


	void setTestEvent(EventBase<TestSender> *event) {
		_testEvent = event;
	}

	int fireEvent() {
		return _testEvent(*this);
	}

};

class TestSender2 : public TestSender {
public:
	int test() {
		return 2;
	}
};

class TestReciever {
public:
	TestReciever() {}
	~TestReciever() {}

	int onTestSend(TestSender &sender) {
		return sender.test();
	}

	void regEvent(TestSender &reg) {
		reg.setTestEvent(NewEvent(*this, &TestReciever::onTestSend));
	}
};


int main() {
	TestSender sender1;
	TestSender2 sender2;
	TestReciever reciever;

	cout << "Registering first event" << endl;
	reciever.regEvent(sender1);
	cout << "Registering second event" << endl;
	reciever.regEvent(sender2);
	cout << "Firing first event" << endl;
	if(sender1.fireEvent() != 1)
		return 1;
	cout << "Firing second event" << endl;
	if(sender2.fireEvent() != 2)
		return 2;

	return 0;
}



