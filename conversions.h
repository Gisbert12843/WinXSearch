#pragma once
#include "wholeinclude.h"

std::wstring StringToWString(const std::string& s);


std::string wide_string_to_string(std::wstring wide_string);

std::string to_lower_string(std::string s);

std::string wide_string_to_string_REF(std::wstring& wide_string);

const wchar_t* GetWC(const char* c);

std::string convertToPath(std::string x);



