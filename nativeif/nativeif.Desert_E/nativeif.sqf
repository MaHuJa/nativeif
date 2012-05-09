player globalchat ( "Native version: " +("nativeif" callextension "version"));
0 spawn compile ("nativeif" callextension "init nativeif");
mhj_nif_spawn = {
	_ref = "nativeif" callextension ("run " + _this);
	diag_log _ref;
	_ref;
};
mhj_nif_call = {
	private ["_query","_ret"];
	_query = "ret " + (_this call mhj_nif_spawn);
	_ret = "";
	while {_ret == ""} do { 
		sleep 0.001; 
		_ret = "nativeif" callextension _query; 
		//player sidechat format [":%1",_query]; 
	};
	0 call compile _ret;
};

mhj_nif_cbinfo = { 
	"nativeif" callextension format ["cbdata %1 %2",_this select 0, _this select 1];
};

0 spawn {
	while {true} do {
		private "_ret";
		_ret = "nativeif" callextension "cb";
		if (_ret == "") then { sleep 0.001; }
		else {
			diag_log "Running CB";
			diag_log _ret;
			0 spawn compile _ret;
		};
	};
};
nativereturn = "BigTestCase" call mhj_nif_call;
player globalchat nativereturn;
diag_log text "Received return:";
diag_log nativereturn;