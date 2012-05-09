#pragma once
#include "implicit_future.h"
#include "rv_callback.h"

namespace nif {
	string escapestring (const string& in);

	template <class T>
	T late(std::shared_ptr<future<string>> input) {
		string s = input->get();
		return T(s);
	}
	template<> int late<int>(std::shared_ptr<future<string>> input);
	template<> float late<float>(std::shared_ptr<future<string>> input);
	template<> bool late<bool>(std::shared_ptr<future<string>> input);

	template<typename T>
	transparent_future<T> query_for (const string& query) {
		std::shared_ptr<future<string>> f1 (new future<string> (rv::sqf_cb(query)));
		return std::async(std::launch::deferred, &late<T>, f1);
	}

} // ns nif

namespace rv {
	using nif::escapestring;
}
