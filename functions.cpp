#include "functions.h"
#include "conversions.h"


int processedFiles = 0;
int processedFolders = 0;

bool skipFiles(std::filesystem::recursive_directory_iterator &iter, std::error_code &ec, int &p_free_threads)
{
	for (int i = 0; i < p_free_threads - 1; i++) //skips x files every turn so all files only get processesed by one thread ever
		iter.increment(ec);
	return true;
}

//given a percentage it prints a nice formatted progressbar to the console
void printProgress(double& percentage)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<std::chrono::microseconds>(stop - start);

	while (percentage < 98)
	{
		stop = std::chrono::high_resolution_clock::now();
		duration = duration_cast<std::chrono::microseconds>(stop - start);
		if (duration.count() >= 300000)
		{
			start = std::chrono::high_resolution_clock::now();
		}
		else
			continue;

		double maxPrint = 20;
		double toPrint = percentage / 5;
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
		std::cout << "]" << percentage << "%\n";
	}
	system("CLS");
	std::cout << "Listing...\n";
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
	int processedFiles,
	int processedFolders,
	auto seconds,
	std::vector<std::filesystem::directory_entry> &vec_folder_path,
	std::vector<std::filesystem::directory_entry> &vec_content_path,
	std::vector<std::filesystem::directory_entry> &vec_file_path)

{
	std::vector<int> vec_to_be_opened; //storing the identifier for files/folders to be opened
	system("CLS");
	int i = 0;

	std::cout << "\nProcessed an astounding " << processedFiles << " files inside of " << processedFolders << " folders in just " << seconds / 1000000 << " seconds.\n\n";


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
			std::cout << x << "\n";
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


void big_for_loop(int p_i, int p_free_threads, double& currentfilecount, const std::wstring& pathToFolder, double& percentage, double& filecount, bool& searchFolders, bool& searchContent,
	std::vector<std::string>& vecSearchValue,
	std::vector<std::filesystem::directory_entry>& vec_folder_path,
	std::vector<std::filesystem::directory_entry>& vec_content_path,
	std::vector<std::filesystem::directory_entry>& vec_file_path)
{
	int frickinFilesCounter = 0;
	int skippedFilesCounter = 0;

	int skipper = 0;
	int onetime_skipped = 0;
	// Iterates over every file/folder in the path of the executable and its subdiretories



	auto iter = std::filesystem::recursive_directory_iterator(pathToFolder, std::filesystem::directory_options::skip_permission_denied);
	auto end_iter = std::filesystem::end(iter);
	auto ec = std::error_code();
	std::filesystem::directory_entry dirEntry;

	for (int i = 0; i < p_i - 1; i++) //onetime skip for each thread (first thread doesnt skip a file ; the 5. does skip 4 files
		iter.increment(ec);

	for (; iter != end_iter; iter.increment(ec))
	{
		dirEntry = std::filesystem::directory_entry(iter->path());
		if (ec)
		{
			continue;
		}
		//std::wcout << dirEntry << "\n";
		// The rest of your loop code here...


		// p_i 0-5
		// free_threads 6

		//case : p_i = 1

		//if (onetime_skipped < p_i - 1)
		//{
		//	onetime_skipped++;
		//	continue;
		//}

		
		if (skipper < p_free_threads)
		{
			if (skipper != 0)
			{
				skipper++;
				continue;
			}
			skipper++;
		}
		if (skipper == p_free_threads)
			skipper = 1;
		 //The Above Block handles that the programm only processes every file just once, by skipping x times at the start (x = 1, if it is the first created thread; = 2 if its the second


		//Skips invisible files
		/*DWORD attributes = GetFileAttributes(LPCWSTR((char*)(std::filesystem::path(dirEntry).c_str())));
		if (attributes & FILE_ATTRIBUTE_HIDDEN)
		{
			continue;
		}*/

		frickinFilesCounter++;
		currentfilecount++;
		percentage = currentfilecount / filecount * 100;




		if ((pathToFolder == dirEntry.path().wstring()) || dirEntry.is_symlink(ec)) //skip if iterated file is the running executable or a symlink
		{
			continue;
		}



		bool found = false;
		if (std::filesystem::is_directory(dirEntry.path())) // optional: folder as iterated file
		{
			if (searchFolders == false)
				continue;

			processedFolders++;

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
		else //since its not a directory, it has to be a file
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
		for (int i = 0; i < p_free_threads-1; i++) //skips x files every turn so all files only get processesed by one thread ever
			iter.increment(ec);
	}// big for loop ending
	processedFiles += frickinFilesCounter;
}



void startWinXSearch(const std::wstring& pathToFolder, bool searchFolders, bool searchContent, std::vector<std::string> vecSearchValue)
{
	auto start = std::chrono::high_resolution_clock::now();

	int current_thread_count = 1;
	const int processor_count = std::thread::hardware_concurrency();



	if (!(std::filesystem::directory_entry(pathToFolder.c_str()).is_directory()))
	{
		std::cout << "PATH IS NOT A DIRECTORY!" << std::endl;
		//std::cin.ignore();
		exit;
	}



	std::vector<std::filesystem::directory_entry> vec_folder_path; //stores folder paths of folders including atleast one of the searched strings
	std::vector<std::filesystem::directory_entry> vec_content_path; //stores file paths of files whichs content includes atleast one of the searched strings
	std::vector<std::filesystem::directory_entry> vec_file_path; //stores file paths of files including atleast one of the searched strings


	double filecount = 0;
	double currentfilecount = 0;
	double percentage = 0;

	std::vector<std::thread>thread_list;


	auto iter = std::filesystem::recursive_directory_iterator(pathToFolder, std::filesystem::directory_options::skip_permission_denied);
	auto end_iter = std::filesystem::end(iter);
	auto ec = std::error_code();


	system("CLS");
	std::cout << "Scanning files\n";
	for (; iter != end_iter; iter.increment(ec))
	{
		if (ec)
		{
			continue;
		}
		filecount++;
	}
	system("CLS");

	std::thread t1(printProgress, std::ref(percentage));
	thread_list.push_back(std::move(t1));
	current_thread_count++;

	int free_threads = processor_count - current_thread_count;

	for (int i = 0; i < free_threads; i++)
	{
		std::thread tnew(big_for_loop,
			std::ref(i),
			std::ref(free_threads),
			std::ref(currentfilecount),
			std::ref(pathToFolder),
			std::ref(percentage),
			std::ref(filecount),
			std::ref(searchFolders),
			std::ref(searchContent),
			std::ref(vecSearchValue),
			std::ref(vec_folder_path),
			std::ref(vec_content_path),
			std::ref(vec_file_path));

		thread_list.push_back(std::move(tnew));
		current_thread_count++;
	}

	system("CLS");

	for (int i = 1; i < thread_list.size(); i++)
	{
		if (thread_list.at(i).joinable())
		thread_list.at(i).join();
	}
	thread_list.at(0).join();

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<std::chrono::microseconds>(stop - start);

	processedFiles -= processedFolders;
	display(processedFiles, processedFolders, duration.count(), vec_folder_path, vec_content_path, vec_file_path);
}

