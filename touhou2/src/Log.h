#pragma once

#ifdef _DEBUG

#include <iostream>

inline void _Log()
{
}

template <typename T, typename... TArgs>
void _Log(T head, TArgs... args)
{
	std::cout << head;
	_Log(args...);
}

template <typename T, typename... TArgs>
void Log(T head, TArgs... args)
{
	_Log(head, args...);
	std::cout << std::endl;
}

inline void Log()
{
	std::cout << std::endl;
}

#else // _DEBUG

template <typename T, typename... TArgs>
void Log(T head, TArgs... args)
{
}

inline void Log()
{
}

#endif // _DEBUG
