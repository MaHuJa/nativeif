#pragma once
#include "implicit_future.h"
#include "rv_util.h"
#include "rv_object.h"

namespace rv {
	void diag_log (string);
	void hint (string);
	transparent_future<Object> player();

	struct Position { 
		float x,y,z;
		Position(string);
		string sqf();
	};
}
