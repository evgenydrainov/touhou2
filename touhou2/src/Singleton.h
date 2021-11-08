#pragma once

template <class T>
class Singleton
{
public:
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator =(const Singleton&) = delete;
	Singleton& operator =(Singleton&&) = delete;

	inline static T& getInstance()
	{
		static T instance{};
		return instance;
	}

protected:
	Singleton() = default;
};
