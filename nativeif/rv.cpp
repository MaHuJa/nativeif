#include "stdafx.h"
#include "nif.h"
#include "rv.h"
#include "rv_callback.h"
#include "rv_util.h"

namespace rv {
	using namespace nif;
void diag_log (string in) {
	ostringstream out;
	out << "diag_log " << escapestring(in) << ";";
	sqf(out.str());
}

void hint (string text) {
	ostringstream out;
	out << "hint " << escapestring(text) << ';';
	sqf(out.str());
}

transparent_future<Object> player() {
	return query_for<Object> ("netid player");
}

Position::Position(string s) {
	istringstream in(s);
	char c;
	in >> c; assert(c=='[');
	in >> x;
	in >> c; assert (c==',');
	in >> y;
	in >> c; assert (c==',');
	in >> z;
	in >> c; assert (c==']');
	if (in.fail()) throw std::logic_error("Invalid position format");
}

string Position::sqf() {
	ostringstream out;
	out << '[' << x << ',' << y << ',' << z << ']';
	return out.str();
}






} //ns rv
