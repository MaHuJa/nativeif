#include "stdafx.h"
#include "nif.h"
#include "rv_callback.h"

namespace nif {
std::ofstream logfile ("nativeif.log");

string escapestring (const string& in) {
	ostringstream out;
	out << '"';
	for_each(begin(in),end(in), [&out](char c) { out << c; if (c=='"') out << c; });
	out << '"';
	return out.str();
}

map<string, UserFunction> callables;
map <std::thread::id,int> threadsessions;	// TODO: Probably need to lock this. Expensive? (Probably not when compared to what we'll be doing when we call this.)
//std::mutex threadsessions_lock;
// Thread local int would do an even better job. 
__declspec( thread ) int threadsession;

map <string,future<string>> threadreturns;
atomic<int> currentsession;
vector<std::thread> allthreads;

// Will never be run by main thread
void session_valid() {
	return;	// For now, don't do this check
	auto it = threadsessions.find(std::this_thread::get_id());
	if (it==threadsessions.end()) throw Session_never_valid();
	if (it->second != currentsession) throw Session_no_longer_valid();
}


// Will only ever be run by main thread
void newsession () {	// Something called us with the init command
	currentsession++;
	std::this_thread::yield();

	{
		std::lock_guard<std::mutex> guard(rv::cbqueue_cb_lock);
		while (!rv::cbqueue_cb.empty()) {
			rv::cbqueue_cb.front().retdata.set_exception(std::make_exception_ptr(Session_no_longer_valid()));
			rv::cbqueue_cb.pop();
		}
	}
	std::for_each (begin(rv::mbox_cb),end(rv::mbox_cb), [] (rv::rv_callback& cb) {
		cb.retdata.set_exception(std::make_exception_ptr(Session_no_longer_valid()));
	});
}

void nif_call_newthread (UserFunction request, string param, std::shared_ptr<promise<string>>& pro) {
	DEBUGLOG("Task thread saying hello");

	// Start point of new threads
	threadsessions[std::this_thread::get_id()] = currentsession;
	DEBUGLOG("Task thread passing control");

	try { 
		string data = request(param);
		pro->set_value(data);
	}
	catch (Session_no_longer_valid&) { }
	catch (Session_never_valid&) { }	// TODO: Consider error message
	return;
}

string nativecall (string command) { 
	DEBUGLOG("nif::nativecall");

	istringstream in(command);
	string name; in >> name;
	auto it = callables.find(name);
	if (it==callables.end()) {
		// TODO: Log event
		return "";
	}
 	in.ignore();
	DEBUGLOG(command << '\n' << command.size() << '/' << in.tellg());
	string param;
	if (in.tellg()>0) param = command.substr(static_cast<unsigned int>(in.tellg()));	

	// Compiler (bind implementation in particular) can't handle passing promises to a new thread, therefore packaged_task

	// Workaround time. Should have been able to just pass move(pro) except for a bug.
	std::shared_ptr<promise<string>> pro ( new promise<string>() );
	auto fut = pro->get_future();
	DEBUGLOG("Starting task thread");
		//t = std::thread (nif_call_newthread, it->second, param, move(pro));
	auto t = std::thread (nif_call_newthread, it->second, param, pro);
	DEBUGLOG("Finished starting task thread");

	ostringstream ref; ref << t.get_id();
	threadreturns[ref.str()]=move(fut);

	allthreads.push_back(move(t));
	return ref.str(); 
}

string get_returnval (string name) { 
	auto it = threadreturns.find(name);
	if (it==threadreturns.end()) 
		return "";
	auto status = it->second.wait_for(std::chrono::nanoseconds(0));
	if (status==std::future_status::ready) 
		return it->second.get();
	return "";
}


} //ns nif
