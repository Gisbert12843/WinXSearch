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
	std::string to_be_searched = "";


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

		std::getline(std::cin, to_be_searched);

	} while (!validateInputStringForInitialInput(to_be_searched, VecSearchValue));



	#ifdef NDEBUG
	{
		startWinXSearch(argv[1], searchFolders, searchContent, VecSearchValue);
	}
	#else
	{
		//std::cout << wide_string_to_string(std::wstring(argv[0]));
		std::string s = "C:\\Sciebo";
		
		startWinXSearch(StringToWString(s), searchFolders, searchContent, VecSearchValue);
	}
	#endif


	CoUninitialize();


}


