#pragma once
#include "wholeinclude.h"

std::wstring StringToWString(const std::string& s);


std::string wide_string_to_string(std::wstring wide_string);

std::string to_lower_string(std::string s);

std::string wide_string_to_string_REF(std::wstring& wide_string);

const wchar_t* GetWC(const char* c);

std::string convertToPath(std::string x);


inline std::mutex& getSharedMutex() {
	static std::mutex sharedMutex;
	return sharedMutex;
}

//this function ensures that whatever thread or translation unit called whatever instantiation of this function will use the same mutex.
//This accomplishes a thread- and "type"-safe way to update a variable in a parallised/multithreaded enviroment.
//template <typename T, typename X>
//void mutual_update(T& to_update, const X& newValue) {
//	getSharedMutex().lock();
//	to_update = newValue;
//	getSharedMutex().unlock();
//
//}

template <typename T, typename X>
void mutual_vector_pushback(std::vector<T>& vector, const X& newValue) {
	getSharedMutex().lock();
	vector.push_back(newValue);
	getSharedMutex().unlock();

}
template <typename T>
void mutual_increment(T& to_update) {
	std::lock_guard<std::mutex> lock(getSharedMutex()); // RAII-style mutex locking
	++to_update; // Increment is now atomic with respect to threads
}