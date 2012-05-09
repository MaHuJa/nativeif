// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "nif.h"
#include "rv_callback.h"
#include "rv.h"


/*	Example of user code
	In proper use this is added to a separate file, and the .h file declaring it is included here.
	Then, it's added in dllmain
	*/
string BigTestCase (string) {
	DEBUGLOG("BigTestCase started");

	using namespace rv;
	Object p = player();
	//string n = p.get_name();
	auto n = p.get_name();
	hint (n);
	n += "!";
	diag_log (n);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	p.set_damage(1);
	return n;
}

extern "C" 
{
  __declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function); 
};

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved ) {
	using namespace nif;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			DEBUGLOG("Attaching DLL");
			callables["BigTestCase"]=BigTestCase;
		}
		break;
	case DLL_PROCESS_DETACH:
			DEBUGLOG("Detaching DLL\n");
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

#define STRTOCS(s) strncpy(output,s.c_str(),outputSize)
void __stdcall RVExtension(char *output, int outputSize, const char *function) {
	outputSize--;	// Don't crash if output exceeds 4k. See strncpy behavior.
	string input = function;	// Lift up from C level ASAP. If performance matters, switch to static_string for C comparable performance.

	if (input=="cb") { 
		STRTOCS(rv::get_callback()); 
		return; 
	}
	if (input=="version") { 
		DEBUGLOG("RVExtension called: "<<function);
		strncpy (output, "0.2", 4); return; 
	}

	if (input.substr(0,4)=="ret ") {
		STRTOCS(nif::get_returnval(input.substr(4)));
		return;
	}
	if (input.substr(0,4)=="run ") {
		DEBUGLOG("RVExtension called: "<<function);
		STRTOCS(nif::nativecall(input.substr(4))) ; 
		return; 
	}
	if (input.substr(0,5)=="init ") {
		DEBUGLOG("RVExtension called: "<<function);
		nif::newsession();
	}
	if (input.substr(0,7)=="cbdata ") { 
		DEBUGLOG("RVExtension called: "<<function);
		rv::set_callback(input.substr(7)); 
		return; 
	}

}

