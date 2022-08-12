#define NOMINMAX

#include <string>
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <fstream>
#include <winbase.h>
#include <numeric>
#include <string_view>
#include <vector>
#include <shlobj_core.h>
#include <time.h>
#include <thread>

//#include <combaseapi.h>

namespace fs = std::filesystem;

using namespace std;


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
	std::cout << "]" << percentage << "%" << endl;
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


bool validateInputStringForInitialInput(string input, vector<string>& output)
{
	string set = "";
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
bool validateInputStringForOpening(string input, vector<int>& output)
{
	string set = "";
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


string convertToPath(string x)
{
	string s = x;
	for (int it = 0; it < s.size(); it++)
	{

		if (s.at(it) == '\\')
		{
			s.replace(it, 2, 2, '\\');
		}
	}
	return s;
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
			std::cout << i + 1 << ": " << wide_string_to_string(vec_folder_path.at(j).path().wstring()) << "\"" << endl;
		std::cout << "****************************************************************\n" << endl;
	}
	if (!vec_file_path.empty())
	{
		std::cout << "Found Files\n****************************************************************\n";
		for (int j = 0; j < vec_file_path.size(); j++, i++)
			std::cout << i + 1 << ": " << wide_string_to_string(vec_file_path.at(j).path().wstring()) << "\"" << endl;
		std::cout << "****************************************************************\n" << endl;
	}
	if (!vec_content_path.empty())
	{
		std::cout << "Found Content\n****************************************************************\n";
		for (int j = 0; j < vec_content_path.size(); j++, i++)
			std::cout << i + 1 << ": " << wide_string_to_string(vec_content_path.at(j).path().wstring()) << "\"" << endl;
		std::cout << "****************************************************************\n" << endl;
	}
	string to_be_opened = ""; //Stringinput of what files the user wants to be opened
	do
	{
		std::cout << "What should be opened? Input-Example: \"1,2,3,40,53\"" << endl;

		std::getline(std::cin, to_be_opened);

	} while (!validateInputStringForOpening(to_be_opened, vec_to_be_opened));



	for (auto& it : vec_to_be_opened)
	{
		if (vec_folder_path.size() >= it)
		{
			auto x = (vec_folder_path.at(it - 1));//	(LPCSTR((char*)(filesystem::path(vec_folder_path.at(it - 1)).c_str())))
			std::cout << x << endl;
			string askdjasd = (wide_string_to_string(filesystem::path(x).wstring()) + "\\\\");
			const char* y = askdjasd.c_str();


			ShellExecuteA(NULL, "open", y, NULL, NULL, SW_SHOWDEFAULT);
		}
		else
			if (vec_file_path.size() + vec_folder_path.size() >= it)
			{
				BrowseToFile(LPCTSTR((char*)(filesystem::path(vec_file_path.at(it - vec_folder_path.size() - 1)).c_str())));
			}
			else
				if (vec_content_path.size() + vec_file_path.size() + vec_folder_path.size() >= it)
				{
					BrowseToFile(LPCTSTR((char*)(filesystem::path(vec_content_path.at(it - 1 - vec_folder_path.size() - vec_file_path.size())).c_str())));
				}


	}

}

void startWinXSearch(const std::wstring& pathToFolder, bool searchFolders, bool searchContent, vector<string> vecSearchValue)
{
	if (!(filesystem::directory_entry(pathToFolder.c_str()).is_directory()))
	{
		cout << "PATH IS NOT A DIRECTORY!" << endl;
		cin.ignore();
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
		string currentPath = wide_string_to_string(currentPathW);

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
				if (string::npos != (wide_string_to_string(dirEntry.path().filename().wstring())).find(it))
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
				if (string::npos != (wide_string_to_string(dirEntry.path().filename().wstring())).find(it)) {
					vec_file_path.push_back(dirEntry);
					found = true;
					break;
				}
			}
			if (found || searchContent == false) //continue iterating if search was successfull on filename or content search is disabled
				continue;




			std::wifstream  inputfile;
			inputfile.open(dirEntry, std::wifstream::in);
			if (inputfile.rdstate() != ios_base::goodbit)
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
						if (string::npos != (wide_string_to_string(it)).find(searchIt))
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
				catch (const std::exception&) { std::cout << "Couldnt close file!" << endl; }
			}
			continue;
		}

	}// big for loop ending
	printProgress((currentfilecount / filecount) * 100);

	std::cout << "DONE" << endl;



	display(vec_folder_path, vec_content_path, vec_file_path);
}








int wmain(int argc, wchar_t* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	bool searchFolders = true; //option for user to search for folders too
	bool searchContent = true; //option for user to search through file content too
	vector<string> VecSearchValue;
	string to_be_opened = "";



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

		cout << "Suchebegriffe mit Leerzeichen getrennt eingeben." << endl;

		std::getline(std::cin, to_be_opened);

	} while (!validateInputStringForInitialInput(to_be_opened, VecSearchValue));

	startWinXSearch(argv[1], searchFolders, searchContent, VecSearchValue);


	CoUninitialize();


}


