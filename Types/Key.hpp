#pragma once
#include <cstdint>

template<typename T>
struct Key
{
public:
	T time;
	T value;
	T forward;
	T backward;
	Key() : time(0.0f), value(0.0f), forward(0.0f), backward(0.0f) {}
	Key(T t, T v, T f, T b) : time(t), value(v), forward(f), backward(b) {}
};