#define NOMINMAX

#include "wholeinclude.h"

#include "conversions.h"

//#include <combaseapi.h>

namespace fs = std::filesystem;





void printProgress(double percentage)
{
	double maxPrint = 20;
	double toPrint = percentage/5;
	system("CLS");
	std::cout << "[";
	for (int i = 0; i < toPrint; i++)
	{
		std::cout << '\|';
		maxPrint--;
	}
	for (int i = 0; i < maxPrint; i++)
	{
		std::cout << ' ';
	}
	std::cout << "]" << percentage << "%" << std::endl;
}




bool validateInputStringForInitialInput(std::string input, std::vector<std::string>& output)
{
	std::string set = "";
	for (int i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ')
		{
			output.push_back(set);
			set = "";
			continue;
		}
		set += input.at(i);
	}
	if (set != "" && set != " ")
	{
		output.push_back(set);
	}
	return true;
}


std::string getFolderPath(std::string pathOfFile)
{
	std::string pathOfFolder = "";
	for (int i = 0; i < pathOfFile.find_last_of("\\"); i++)
	{
		pathOfFolder += pathOfFile.at(i);
	}
	return pathOfFolder;
}



void BrowseToFile(LPCWSTR filename)
{
	ITEMIDLIST* pidl = ILCreateFromPathW(filename);
	if (pidl) {
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
	}
}

void BrowseToFolder(LPCWSTR filename) //thx to "Jonathan Potter" on stackoverflow
{
	PIDLIST_ABSOLUTE pidl;
	if (SUCCEEDED(SHParseDisplayName(filename, 0, &pidl, 0, 0)))
	{
		// we don't want to actually select anything in the folder, so we pass an empty
		// PIDL in the array. if you want to select one or more items in the opened
		// folder you'd need to build the PIDL array appropriately
		ITEMIDLIST idNull = { 0 };
		LPCITEMIDLIST pidlNull[1] = { &idNull };
		SHOpenFolderAndSelectItems(pidl, 1, pidlNull, 0);
		ILFree(pidl);
	}
}



//validates inputstring (returns false if wrong input) and transfers input string to vector
bool validateInputStringForOpening(std::string input, std::vector<int>& output)
{
	std::string set = "";
	for (int i = 0; i < input.length(); i++)
	{
		if (isdigit(input.at(i)))
		{
			set += (input.at(i));
		}
		else
			if (input.at(i) == ',' || input.at(i) == ' ')
			{
				output.push_back(stoi(set));
				set = "";
				continue;
			}
			else
				return false;
	}
	if (set != "")
	{
		output.push_back(stoi(set));
	}
	return true;
}




void display(
	std::vector<std::filesystem::directory_entry> vec_folder_path,
	std::vector<std::filesystem::directory_entry> vec_content_path,
	std::vector<std::filesystem::directory_entry> vec_file_path)

{


	std::vector<int> vec_to_be_opened; //storing the identifier for files/folders to be opened
	system("CLS");
	int i = 0;
	if (!vec_folder_path.empty())
	{
		std::cout << "Found Folders\n****************************************************************\n";
		for (int j = 0; j < vec_folder_path.size(); j++, i++)
			std::cout << i + 1 << ": " << wide_string_to_string(vec_folder_path.at(j).path().wstring()) << "\"" << std::endl;
		std::cout << "****************************************************************\n" << std::endl;
	}
	if (!vec_file_path.empty())
	{
		std::cout << "Found Files\n****************************************************************\n";
		for (int j = 0; j < vec_file_path.size(); j++, i++)
			std::cout << i + 1 << ": " << wide_string_to_string(vec_file_path.at(j).path().wstring()) << "\"" << std::endl;
		std::cout << "****************************************************************\n" << std::endl;
	}
	if (!vec_content_path.empty())
	{
		std::cout << "Found Content\n****************************************************************\n";
		for (int j = 0; j < vec_content_path.size(); j++, i++)
			std::cout << i + 1 << ": " << wide_string_to_string(vec_content_path.at(j).path().wstring()) << "\"" << std::endl;
		std::cout << "****************************************************************\n" << std::endl;
	}
	std::string to_be_opened = ""; //Stringinput of what files the user wants to be opened

	if (!(vec_content_path.empty() && vec_file_path.empty() && vec_folder_path.empty()))
	{
		do
		{
			std::cout << "What should be opened? Input-Example: \"1,2,3,40,53\"" << std::endl;

			std::getline(std::cin, to_be_opened);

		} while (!validateInputStringForOpening(to_be_opened, vec_to_be_opened));
	}
	else
	{
		std::cout << "No files match the searching criteria :(" << std::endl;
		std::cout << "Press \"Enter\" to exit.";
		std::cin.ignore();
		exit;
	}


	for (auto& it : vec_to_be_opened)
	{
		if (vec_folder_path.size() >= it)
		{
			auto x = (vec_folder_path.at(it - 1));//	(LPCSTR((char*)(filesystem::path(vec_folder_path.at(it - 1)).c_str())))
			std::cout << x << std::endl;
			std::string askdjasd = (wide_string_to_string(std::filesystem::path(x).wstring()) + "\\\\");
			const char* y = askdjasd.c_str();


			ShellExecuteA(NULL, "open", y, NULL, NULL, SW_SHOWDEFAULT);
		}
		else
			if (vec_file_path.size() + vec_folder_path.size() >= it)
			{
				BrowseToFile(LPCTSTR((char*)(std::filesystem::path(vec_file_path.at(it - vec_folder_path.size() - 1)).c_str())));
			}
			else
				if (vec_content_path.size() + vec_file_path.size() + vec_folder_path.size() >= it)
				{
					BrowseToFile(LPCTSTR((char*)(std::filesystem::path(vec_content_path.at(it - 1 - vec_folder_path.size() - vec_file_path.size())).c_str())));
				}


	}

}

void startWinXSearch(const std::wstring& pathToFolder, bool searchFolders, bool searchContent, std::vector<std::string> vecSearchValue)
{
	if (!(std::filesystem::directory_entry(pathToFolder.c_str()).is_directory()))
	{
		std::cout << "PATH IS NOT A DIRECTORY!" << std::endl;
		std::cin.ignore();
		exit;
	}

	std::vector<std::filesystem::directory_entry> vec_folder_path; //stores folder paths of folders including atleast one of the searched strings
	std::vector<std::filesystem::directory_entry> vec_content_path; //stores file paths of files whichs content includes atleast one of the searched strings
	std::vector<std::filesystem::directory_entry> vec_file_path; //stores file paths of files including atleast one of the searched strings

	double filecount = 0;
	double currentfilecount = 0;

	for (const auto& file : fs::recursive_directory_iterator(pathToFolder))
		filecount++;

	int milli_seconds = 1 * 1000;
	time_t start, end;
	start = time(0);

	for (auto& dirEntry : fs::recursive_directory_iterator(pathToFolder)) // Iterates over every file/folder in the path of the executable and its subdiretories
	{
		currentfilecount++;
		if (time(0) - start == 1)
		{
			printProgress((currentfilecount / filecount) * 100);
			start = start + 1;
		}

		bool found = false;
		std::wstring currentPathW = dirEntry.path().c_str();
		std::string currentPath = wide_string_to_string(currentPathW);

		//std::cout << currentPath << endl;
		//std::wcout << currentPathW << endl;
		//wcout.flush(); wcout.clear();


		if ((pathToFolder == dirEntry.path().wstring()) || dirEntry.is_symlink()) //skip if iterated file is the running executable or a symlink
			continue;


		if (std::filesystem::is_directory(dirEntry.path())) // optional: folder as iterated file
		{
			if (searchFolders == false)
				continue;

			for (auto& it : vecSearchValue)
			{
				if (std::string::npos != (to_lower_string(wide_string_to_string(dirEntry.path().filename().wstring()))).find(it))
				{
					vec_folder_path.push_back(dirEntry);
					found = true;
					break;
				}
			}
			continue;
		}
		else
		{
			for (auto& it : vecSearchValue)
			{
				if (std::string::npos != (to_lower_string(wide_string_to_string(dirEntry.path().filename().wstring()))).find(it)) {
					vec_file_path.push_back(dirEntry);
					found = true;
					break;
				}
			}
			if (found || searchContent == false) //continue iterating if search was successfull on filename or content search is disabled
				continue;




			std::wifstream  inputfile;
			inputfile.open(dirEntry, std::wifstream::in);
			if (inputfile.rdstate() != std::ios_base::goodbit)
				continue;


			if (inputfile.good() && inputfile) // optional: filecontent as target
			{

				std::wstring checkedString;
				std::vector<std::wstring> vecOfStr; //File Content as Vector of Strings

				while (std::getline(inputfile, checkedString)) // Every line of the inputfile gets its own string in the vector
				{
					vecOfStr.push_back(checkedString);
				}
				inputfile.close();



				for (auto& it : vecOfStr) // checks every string of the vector for occurences of the wanted word and replaces it
				{
					for (auto& searchIt : vecSearchValue)
					{
						if (std::string::npos != (to_lower_string(wide_string_to_string(it))).find(searchIt))
						{
							vec_content_path.push_back(dirEntry);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
			}
			else
			{
				try { inputfile.close(); }
				catch (const std::exception&) { std::cout << "Couldnt close file!" << std::endl; }
			}
			continue;
		}

	}// big for loop ending
	printProgress((currentfilecount / filecount) * 100);

	std::cout << "DONE" << std::endl;



	display(vec_folder_path, vec_content_path, vec_file_path);
}








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


