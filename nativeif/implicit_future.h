#pragma once

/*	Implicit future:
	The point is to delay the get as long as possible; but otherwise act as if we did not do anything special.

	E.g.
	int a = getval();
	auto b = getval();
	// do stuff
	return a + b + c;	// Don't actually wait for b until you get here.

	Undesirable side effects include:
		b += c;	// Can't write to me
	and
		useval(b);
		useval(b);	// Oops, no longer valid

	*/

template <class T>
class implicit_future :
	public std::future<T>
{
public:
	operator T&& () {
		return get();
	}
	implicit_future (future<T>&& rhs) : future (move(rhs)) {};
};

/*	transparent_future
	made to rectify the shortcomings implicit_future
	*/

template <class T>
class transparent_future {
	std::future<T> fut;
	T value;
	bool acquired;
	inline void get() { if (!acquired) value = fut.get(); }
public:
	transparent_future(std::future<T>&& futr) : fut(move(futr)), acquired(false) {}
	transparent_future(T present) : fut(), value(present), acquired(true) {};
	operator T& () { 
		get();
		return value;
	}
	template <class Y>	T& operator= (Y rhs) { get(); value = rhs; return value; }
	template <class Y>	T& operator+= (Y rhs) { get(); value = rhs; return value; }
	template <class Y>	T& operator-= (Y rhs) { get(); value = rhs; return value; }
	template <class Y>	T& operator*= (Y rhs) { get(); value = rhs; return value; }
	template <class Y>	T& operator/= (Y rhs) { get(); value = rhs; return value; }
	template <class Y>	T& operator++ () { get(); return ++value; }
	template <class Y>	T operator++ (int) { get(); return value++; }
};

