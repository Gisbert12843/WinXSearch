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
//#include <combaseapi.h>

namespace fs = std::filesystem;

using namespace std;

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
		cout << "Found Folders\n****************************************************************\n";
		for (int j = 0; j < vec_folder_path.size(); j++, i++)
			cout << i + 1 << ": " << vec_folder_path.at(j) << "\"" << endl;
		cout << "****************************************************************\n" << endl;
	}
	if (!vec_file_path.empty())
	{
		cout << "Found Files\n****************************************************************\n";
		for (int j = 0; j < vec_file_path.size(); j++, i++)
			cout << i + 1 << ": " << vec_file_path.at(j) << "\"" << endl;
		cout << "****************************************************************\n" << endl;
	}
	if (!vec_content_path.empty())
	{
		cout << "Found Content\n****************************************************************\n";
		for (int j = 0; j < vec_content_path.size(); j++, i++)
			cout << i + 1 << ": " << vec_content_path.at(j) << "\"" << endl;
		cout << "****************************************************************\n" << endl;
	}
	string to_be_opened = ""; //Stringinput of what files the user wants to be opened
	do
	{
		cout << "Was soll geöffnet werden? Eingabemuster: \"1,2,3,40,53\"" << endl;

		std::getline(std::cin, to_be_opened);

	} while (!validateInputStringForOpening(to_be_opened, vec_to_be_opened));



	for (auto& it : vec_to_be_opened)
	{
		if (vec_folder_path.size() >= it)
		{
			auto x = (vec_folder_path.at(it - 1));//	(LPCSTR((char*)(filesystem::path(vec_folder_path.at(it - 1)).c_str())))
			cout << x << endl;
			string askdjasd = (filesystem::path(x).string()) + "\\\\";
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

void startWinXSearch(string argv, bool searchFolders, bool searchContent, vector<string> vecSearchValue)
{
	//string pathToFolder = getFolderPath(argv);
	string pathToFolder = argv;
	if (!filesystem::directory_entry(argv).is_directory())
	{
		pathToFolder = getFolderPath(pathToFolder);
	}
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator; // for convenience


	std::vector<std::filesystem::directory_entry> vec_folder_path; //stores folder paths of folders including atleast one of the searched strings
	std::vector<std::filesystem::directory_entry> vec_content_path; //stores file paths of files whichs content includes atleast one of the searched strings
	std::vector<std::filesystem::directory_entry> vec_file_path; //stores file paths of files including atleast one of the searched strings


	cout << pathToFolder << endl;
	//cin.ignore();

	for (auto dirEntry : recursive_directory_iterator(pathToFolder)) // Iterates over every file/folder in the path of the executable and its subdiretories
	{
		bool found = false;

		//cout << dirEntry << endl;
		cout << dirEntry.path().string() << endl;
		//cin.ignore();

		if ((argv == dirEntry.path().string()) || dirEntry.is_symlink()) //skip if iterated file is the running executable
			continue;


		if (std::filesystem::is_directory(dirEntry.path())) // optional: folder as iterated file
		{
			if (searchFolders == false)
				continue;

			for (auto& it : vecSearchValue)
			{
				if (string::npos != dirEntry.path().filename().string().find(it))
				{
					vec_folder_path.push_back(dirEntry);
					found = true;
					break;
				}
			}
		}
		else
		{
			for (auto& it : vecSearchValue)
			{
				if (string::npos != (dirEntry.path().filename().string()).find(it))
				{
					vec_file_path.push_back(dirEntry);
					found = true;
					break;
				}
			}
			if (found)
				continue;

			std::ifstream inputfile;
			inputfile.open(dirEntry, std::ifstream::in);
			if (inputfile.rdstate() != ios_base::goodbit)
				continue;
			if (searchContent == true && inputfile.good() && inputfile) // optional: filecontent as target
			{

				std::string checkedString;
				std::vector<std::string> vecOfStr; //File Content as Vector of Strings

				while (std::getline(inputfile, checkedString)) // Every line of the inputfile gets its own string in the vector
				{
					vecOfStr.push_back(checkedString);
				}
				inputfile.close();



				for (auto& it : vecOfStr) // checks every string of the vector for occurences of the wanted word and replaces it
				{
					for (auto& searchIt : vecSearchValue)
					{
						if (string::npos != it.find(searchIt))
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
				catch (const std::exception&) { cout << "Couldnt close file!" << endl; }
			}
		}
	}
	display(vec_folder_path, vec_content_path, vec_file_path);

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

int main(int argc, char* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	bool searchFolders = true; //option for user to search for folders too
	bool searchContent = false; //option for user to search through file content too
	vector<string> VecSearchValue;
	string to_be_opened = "";


	string test = "C:\\Sciebo";
	//string test = "C:\\Sciebo\\Projects\\WinXSearch\\x64\\Debug\\WinXSearch.exe";


	do
	{

		cout << "Suchebegriffe mit Leerzeichen getrennt eingeben." << endl;

		std::getline(std::cin, to_be_opened);

	} while (!validateInputStringForInitialInput(to_be_opened, VecSearchValue));


	if (argc == 1)
	{
		cout << "starting argv0" << endl;
		startWinXSearch(test, searchFolders, searchContent, VecSearchValue);
		return 1;
	}
	else
		if (argc == 2)
		{
			startWinXSearch(argv[1], searchFolders, searchContent, VecSearchValue);
		}

	//for (int i = 1; i < argc; i++) // processes Argv inputs, either optional setting (-F / -C) or search strings
	//{
	//	string s = argv[i];
	//	switch (i)
	//	{
	//	case 1:
	//	case 2:
	//	{
	//		if (s == "-F" || s == "-f")
	//		{
	//			cout << "F\n";
	//			searchFolders = true;
	//			break;
	//		}
	//		if (s == "-C" || s == "-c")
	//		{
	//			cout << "C\n";
	//			searchContent = true;
	//			break;
	//		}
	//	}
	//	default:
	//	{
	//		string s = "";
	//		for (int j = 0; j < strlen(argv[i]); j++)
	//		{
	//			s += argv[i][j];
	//		}
	//		VecSearchValue.push_back(s);
	//		cout << "added " << s << "\n";
	//		break;
	//	}
	//	}
	//}

}


