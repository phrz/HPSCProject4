// [CITE] http://coliru.stacked-crooked.com/a/92a50828d6cb6f01

#ifndef _PRINTLN_CPP_
#define _PRINTLN_CPP_

#include <iostream>

template<typename T>
void println(const T& t) {
	std::cout << t << std::endl;
}

template<typename... Args>
void println(Args... a);

template<typename T, typename... Args>
void println(const T& t, const Args&... a) {
	std::cout << t << " ";
	println(a...);
}

template<>
void println() {
	std::cout << std::endl;
}

#endif
