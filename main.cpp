#define NOMINMAX

#include "wholeinclude.h"

#include "conversions.h"
#include "functions.h"













int wmain(int argc, wchar_t* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	bool searchFolders = true; //option for user to search for folders too
	bool searchContent = true; //option for user to search through file content too
	std::vector<std::string> VecSearchValue;
	std::string to_be_opened = "";



	if (argc == 3)
	{
		if (argv[2] == L"-F" || argv[2] == L"-f")
			searchFolders = true;
		else
			if (argv[2] == L"-C" || argv[2] == L"-c")
				searchContent = true;
	}
	if (argc == 4)
	{
		if (argv[3] == L"-F" || argv[3] == L"-f")
			searchFolders = true;
		else
			if (argv[3] == L"-C" || argv[3] == L"-c")
				searchContent = true;
	}

	do
	{

		std::cout << "Please provide Strings to be searched for. Seperated by spaces." << std::endl;

		std::getline(std::cin, to_be_opened);

	} while (!validateInputStringForInitialInput(to_be_opened, VecSearchValue));

	startWinXSearch(argv[1], searchFolders, searchContent, VecSearchValue);


	CoUninitialize();


}


