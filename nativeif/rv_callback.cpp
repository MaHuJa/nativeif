#include "stdafx.h"
#include "nif.h"
#include "rv_callback.h"

namespace rv {

typedef std::lock_guard<std::mutex> lock;
atomic<int> callbackid_counter = 1;

// Lockless queues would be an improvement
queue<rv_callback> cbqueue_cb;
std::mutex cbqueue_cb_lock;


void sqf(string command) {
	nif::session_valid();
	rv_callback cb;
	cb.sqf = command;
	cb.id = 0;
	lock guard(cbqueue_cb_lock);
	cbqueue_cb.push(move(cb));
}

std::future<string> sqf_cb(string command) {
	nif::session_valid();
	rv_callback cb;
	cb.sqf = command;
	auto ret = cb.retdata.get_future();
	cb.id = callbackid_counter.fetch_add(1);

	ostringstream out;
	{
		lock guard(cbqueue_cb_lock);
		cbqueue_cb.push(move(cb));
	}
	return ret;
}

/*	Usually, this list will only have one item - sqf will fullfill the cb and return before asking for a new one.
	If there are more threads asking, and one is paused in or during execution, this can grow. 
	Or if one of the callback tasks include stuff that takes some time.

	In the absense of that, the simpler implementation is to skip the list and id, 
	and merely refuse to give another cb when an answer is pending.
	*/

list<rv_callback> mbox_cb;
// No lock, purely main thread accessed

string get_callback() {	// Called from main thread
	rv_callback cb;
	{
		lock guard (cbqueue_cb_lock);
		if (cbqueue_cb.empty()) return "";	// TODO: Figure out exact protocol
		cb = move(cbqueue_cb.front());
		cbqueue_cb.pop();
	}
	DEBUGLOG("Callback picked up: " << cb.id);
	if (cb.id) { 
		ostringstream out;
		out << "[" << cb.id << ", 0 call {" << cb.sqf << "}] call mhj_nif_cbinfo; ";	// TODO: get name of self (init call?), send as 3rd parameter.
		mbox_cb.push_back(move(cb));
		return out.str();
	} else {
		return cb.sqf;
	}
}

void set_callback(string value) {	// Called from main thread with result
	DEBUGLOG("CB result: " << value);
	istringstream in (value);
	int id; in >> id;
	for (auto it = mbox_cb.begin(); it!=mbox_cb.end(); it++) {
		if (it->id != id) continue;
		
		int offset = static_cast<int>(in.tellg()) + 1;
		string param;
		if (offset>0) param = value.substr(offset);

		it->retdata.set_value(param);	// +1 to skip space
		mbox_cb.erase(it);
		break;
	}
}


} //ns rv
