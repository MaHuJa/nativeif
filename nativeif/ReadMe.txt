

Features:

- Native standard C++, modern style, meant for static compilation. 

- Do a native call, get a return value later. If ever.
- Calls to engine block to get the information needed; std::async() can bypass that as per standard C++. 
  (Async could help with performance. Will look into that.)
- Writing the native functions require no special consideration for session end/change. 
  A couple exceptions must remain untrapped by user code, and is only triggered by activity towards the game engine.
  (Not quite done)

- Approximately 3 lines of C (non++) code. 


Test sqf:
mhj_nif_spawn 
mhj_nif_call which blocks until the function finishes, and returns the return value.

nativereturn = "BigTestCase" call mhj_nif_call;
player globalchat nativereturn;	//actually, player is probably dead at this point. Should switch to titletext or some such.
diag_log text "Received return:";
diag_log nativereturn;


Test C++ code

string BigTestCase (string) {
	using namespace rv;
	Object p = player();
	string n = p.get_name();
	hint (n);
	diag_log (n);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	p.set_damage(1);
	return n;
}
// In dllmain process attach
// callables["BigTestCase"]=BigTestCase;
