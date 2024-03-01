#define NOMINMAX

#include "wholeinclude.h"

#include "conversions.h"
#include "functions.h"


int wmain(int argc, wchar_t* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	bool searchFolders = false; //option for user to search for folders too
	bool searchContent = false; //option for user to search through file content too
	std::vector<std::string> VecSearchValue;
	std::string to_be_searched = "";


	if (argc == 3)
	{
		if (argv[2] == L"-F" || argv[2] == L"-f")
		{
			searchFolders = true;
			std::cout << "Searching for Foldernames - Activated\n";
		}
		else
			if (argv[2] == L"-C" || argv[2] == L"-c")
			{
				searchContent = true;
				std::cout << "Searching in Filecontent - Activated\n";
			}
	}
	else if (argc == 4)
	{
		if (argv[3] == L"-F" || argv[3] == L"-f")
		{
			searchFolders = true;
			std::cout << "Searching for Foldernames - Activated\n";
		}
		else
			if (argv[3] == L"-C" || argv[3] == L"-c")
			{
				searchContent = true;
				std::cout << "Searching in Filecontent - Activated\n";
			}
	}
	else
	{
		std::cout << "Searching for Foldernames - Deactivated\n";
		std::cout << "Searching in Filecontent - Deactivated\n";
	}

	do
	{

		std::cout << "Please provide Strings to be searched for. Seperated by spaces." << std::endl;

		std::getline(std::cin, to_be_searched);

	} while (!validateInputStringForInitialInput(to_be_searched, VecSearchValue));


	std::cout << "Searching for: \"" << VecSearchValue.at(0) << "\"\n";

#ifdef NDEBUG
	{
		//std::cout << wide_string_to_string(std::wstring(argv[0]));
		//std::string s = "C:\\Users\\wwwgi\\Music\\New folder";
		//s = "C:\\Windows";
		//startWinXSearch(StringToWString(s), searchFolders, searchContent, VecSearchValue);
		do_log << "Start search in: \"" << std::filesystem::path(argv[1]).string() << "\"\n";



		startWinXSearch(std::filesystem::path(argv[1]), 1, 1, VecSearchValue);
	}
#endif

#ifndef NDEBUG
	{
		//std::cout << wide_string_to_string(std::wstring(argv[0]));
		std::string s = "C:\\Users\\Kai\\Sciebo\\Projects\\Gisberts_STL\\mqtt_library";
		//s = "C:\\Program Files";
		do_log << "Start search in: \""<< std::filesystem::path(s).string() << "\"\n";

		startWinXSearch(std::filesystem::path(s), true, true, VecSearchValue);
	}
#endif


	CoUninitialize();


}


