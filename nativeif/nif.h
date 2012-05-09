#pragma once

namespace nif {

typedef std::function<string(string)> UserFunction;
class Session_no_longer_valid : public std::exception {};
class Session_never_valid : public std::exception {};

void session_valid();
void newsession();

string nativecall (string);	// "function parameter---"
string get_returnval (string);

// Implementation stuff
extern map<string, UserFunction> callables;

#ifdef _DEBUG
extern std::ofstream logfile;
#define DEBUGLOG(s) nif::logfile << s << std::endl
#else
#define DEBUGLOG(s)
#endif

}// ns nif
