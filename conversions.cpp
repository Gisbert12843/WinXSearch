#include "conversions.h"

std::string wide_string_to_string(std::wstring wide_string)
{


	if (wide_string.empty())
	{
		return "";
	}


	const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), nullptr, 0, nullptr, nullptr);
	if (size_needed <= 0)
	{
		throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
	}

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), &result.at(0), size_needed, nullptr, nullptr);



	return result;
}

std::string to_lower_string(std::string s)
{
	for (int i = 0; i < s.size(); i++)
	{
		s.at(i) = towlower(s.at(i));
	}
	return s;
}

std::string wide_string_to_string_REF(std::wstring& wide_string)
{
	if (wide_string.empty())
	{
		return "";
	}

	const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), nullptr, 0, nullptr, nullptr);
	if (size_needed <= 0)
	{
		throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
	}

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), &result.at(0), size_needed, nullptr, nullptr);
	return result;
}

const wchar_t* GetWC(const char* c)
{
	size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];

	wchar_t* wc2 = wc;
	delete[] wc;

	size_t outSize;
	mbstowcs_s(&outSize, wc2, cSize, c, cSize - 1);



	return wc2;
}

std::string convertToPath(std::string x)
{
	std::string s = x;
	for (int it = 0; it < s.size(); it++)
	{

		if (s.at(it) == '\\')
		{
			s.replace(it, 2, 2, '\\');
		}
	}
	return s;
}