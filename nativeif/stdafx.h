#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
// Windows Header Files:
#include <windows.h>

#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
using std::istringstream;
#include <fstream>
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
#include <queue>
using std::queue;

using std::move;

#include <memory>

#include <algorithm>
using std::for_each;
#include <functional>
// using std::function;

#include <thread>
#include <atomic>
using std::atomic;
#include <future>
using std::promise;
using std::future;

#include <condition_variable>
#include <mutex>

// Sleep, future status
#include <chrono>

#include <stdexcept>
#include <cassert>
