#define NOMINMAX

#include "wholeinclude.h"

#include "conversions.h"
#include "functions.h"


int main(int argc, char* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	bool searchFolders = false; //option for user to search for folders too
	bool searchContent = false; //option for user to search through file content too
	std::vector<std::string> VecSearchValue;
	std::string to_be_searched = "";



	for (uint64_t i = 2; i < argc; i++)
	{
		if (std::string(argv[i]) == std::string("-f"))
		{
			searchFolders = true;
			debug_log << "Searching for Foldernames - Activated\n";
		}
		else if (std::string(argv[i]) == std::string("-F"))
		{
			searchFolders = true;
			debug_log << "Searching for Foldernames - Activated\n";
		}
		else if (std::string(argv[i]) == std::string("-c"))
		{
			searchContent = true;
			debug_log << "Searching in Filecontent - Activated\n";
		}
		else if (std::string(argv[i]) == std::string("-C"))
		{
			searchContent = true;
			debug_log << "Searching in Filecontent - Activated\n";
		}
	}


	std::cout << "Please provide your search Strings. Press Enter after every String. Empty Input == Continue with Search." << std::endl;
	do
	{
		std::getline(std::cin, to_be_searched);

	} while (!validateInputStringForInitialInput(to_be_searched, VecSearchValue));


	for (auto& it : VecSearchValue)
	{
		std::cout << "Searching for: \"" << it << "\"\n";
	}

#ifdef NDEBUG
	{

		//std::cout << std::string(argv[0]) << "\n";
		//std::cout << std::string(argv[1]) << "\n";
		//std::cout << std::string(argv[2]) << "\n";
		//std::cout << std::string(argv[3]) << "\n";
		//std::cout << searchFolders << "\n";
		//std::cout << searchContent << "\n";
		std::cout << "Mode:\n\t";

		if (searchFolders)
			std::cout << "Search for Foldernames.\n\t";
		else
			std::cout << "Searching for Foldernames - Deactivated\n\t";

		if (searchContent)
			std::cout << "Search in Filecontent.\n\t";
		else
			std::cout << "Searching in Filecontent - Deactivated\n\t";


		startWinXSearch(std::filesystem::path(argv[1]), searchFolders, searchContent, VecSearchValue);

	}
#endif

#ifndef NDEBUG
	{
		//std::cout << wide_string_to_string(std::wstring(argv[0]));
		std::string s = "C:\\Users\\Kai\\Sciebo\\Studium";
		//s = "C:\\Program Files";
		debug_log << "Start search in: \"" << std::filesystem::path(s).string() << "\"\n";

		startWinXSearch(std::filesystem::path(s), 1, 1, VecSearchValue);
	}
#endif


	CoUninitialize();


}


