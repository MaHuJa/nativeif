#include "stdafx.h"

#include "rv_util.h"

namespace nif {
	template<>
	int late<int>(std::shared_ptr<future<string>> input) {
		string s = input->get();
		istringstream in (s);
		int i; in >> i;
		if (in.fail()) throw std::runtime_error("Expected int got " + s);
		return i;
	}
	template<>
	float late<float>(std::shared_ptr<future<string>> input) {
		string s = input->get();
		istringstream in (s);
		float f; in >> f;
		if (in.fail()) throw std::runtime_error("Expected int got " + s);
		return f;
	}
	template<>
	bool late<bool>(std::shared_ptr<future<string>> input) {
		string s = input->get();
		if (s == "true") return true;
		return false;
	}

}
