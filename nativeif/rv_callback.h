#pragma once

// TODO: Put user off limits stuff in namespace nif

namespace rv {

void sqf(string);
std::future<string> sqf_cb(string);	// This function is low-level, and only intended for use by other API functions.

string get_callback();	// Called from main thread
void set_callback(string value);	// Called from main thread with result

// Callback queue - information requests
struct rv_callback {
	int id;	
	string sqf;
	std::promise<string> retdata;

	rv_callback() {};
	rv_callback(rv_callback&& rhs) : id(rhs.id), sqf(move(rhs.sqf)), retdata(move(rhs.retdata)) {}
	rv_callback& operator= (rv_callback&& rhs) { id = rhs.id; sqf = move(rhs.sqf); retdata = move(rhs.retdata); return *this; }
private:
	rv_callback(const rv_callback&); // = delete;
	rv_callback& operator= (const rv_callback&); // = delete;
};

extern queue<rv_callback> cbqueue_cb;
extern std::mutex cbqueue_cb_lock;
extern list<rv_callback> mbox_cb;

}; //ns rv
