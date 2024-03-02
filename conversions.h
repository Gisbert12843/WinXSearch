#pragma once
#include "wholeinclude.h"

std::wstring StringToWString(const std::string& s);


std::string wide_string_to_string(std::wstring wide_string);

std::string to_lower_string(std::string s);

std::string wide_string_to_string_REF(std::wstring& wide_string);

const wchar_t* GetWC(const char* c);

std::string convertToPath(std::string x);


inline std::mutex& getGlobalMutex() {
    static std::mutex mutex;
    return mutex;
}

//this function ensures that whatever thread or translation unit called whatever instantiation of this function will use the same mutex.
//This accomplishes a thread- and "type"-safe way to update a variable in a parallised/multithreaded enviroment.
template <typename T, typename X>
void mutual_update(T& to_update, const X& newValue) {
    std::lock_guard<std::mutex> lock(getGlobalMutex());
    to_update = newValue;
}