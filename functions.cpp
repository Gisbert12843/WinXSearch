#include "functions.h"
#include "conversions.h"

//void big_for_loop(int p_i, int total_threads, double& currentfilecount, const std::filesystem::path& pathToFolder, double& percentage, double& filecount, bool& searchFolders, bool& searchContent,
//	std::vector<std::string>& vecSearchValue,
//	std::vector<std::filesystem::directory_entry>& vec_folder_path,
//	std::vector<std::filesystem::directory_entry>& vec_content_path,
//	std::vector<std::filesystem::directory_entry>& vec_file_path);


int processedFiles = 0;
int processedFolders = 0;

bool skipFiles(std::filesystem::recursive_directory_iterator& iter, std::error_code& ec, int& p_free_threads)
{
	for (int i = 0; i < p_free_threads - 1; i++) //skips x files every turn so all files only get processesed by one thread ever
		iter.increment(ec);
	return true;
}

//given a percentage it prints a nice formatted progressbar to the console

//TODO	Let it track a static int of how many chars have been written the last time, then just set position to 0,0 and overwrite the locations to whatever string to write
//		is longer the old one or the new one
//void printProgress(double& percentage)
//{
//	auto start = std::chrono::high_resolution_clock::now();
//	auto stop = std::chrono::high_resolution_clock::now();
//	auto duration = duration_cast<std::chrono::microseconds>(stop - start);
//
//	while (percentage < 98)
//	{
//		stop = std::chrono::high_resolution_clock::now();
//		duration = duration_cast<std::chrono::microseconds>(stop - start);
//		if (duration.count() >= 300000)
//		{
//			start = std::chrono::high_resolution_clock::now();
//		}
//		else
//			continue;
//
//		double maxPrint = 20;
//		double toPrint = percentage / 5;
//		system("CLS");
//		std::cout << "[";
//		for (int i = 0; i < toPrint; i++)
//		{
//			std::cout << '\|';
//			maxPrint--;
//		}
//		for (int i = 0; i < maxPrint; i++)
//		{
//			std::cout << ' ';
//		}
//		std::cout << "]" << percentage << "%\n";
//	}
//	system("CLS");
//	std::cout << "Listing...\n";
//}

void printProgress(double& percentage)
{
	while (percentage < 98)
	{
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
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	system("CLS");
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
	std::vector<std::filesystem::directory_entry>& vec_folder_path,
	std::vector<std::filesystem::directory_entry>& vec_content_path,
	std::vector<std::filesystem::directory_entry>& vec_file_path)

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




void startWinXSearch(const std::filesystem::path pathToFolder, bool searchFolders, bool searchContent, std::vector<std::string> vecSearchValue)
{
	auto start = std::chrono::high_resolution_clock::now();

	int current_thread_count = 1;
	int processor_count = std::thread::hardware_concurrency();

	debug_log << "Found " << processor_count << " CPUs.\n";


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
	double percentage = 0;

	std::vector<std::thread>thread_list;


	auto iter = std::filesystem::recursive_directory_iterator(pathToFolder, std::filesystem::directory_options::skip_permission_denied);
	auto end_iter = std::filesystem::end(iter);
	auto ec = std::error_code();


	//Calculates Amount of Files
	//system("CLS");
	std::cout << "Scanning files\n";
	for (; iter != end_iter; iter.increment(ec))
	{
		if (ec)
		{
			continue;
		}
		filecount++;
	}
	//system("CLS");


	//Starts ProgressBar Thread
	//std::thread t1(printProgress, std::ref(percentage));
	//thread_list.push_back(std::move(t1));
	//current_thread_count++;

	debug_log << "Starting big_for_loop\n";

	//std::thread tnew(big_for_loop,
	//	1,
	//	1,
	//	std::ref(currentfilecount),
	//	std::ref(pathToFolder),
	//	std::ref(percentage),
	//	std::ref(filecount),
	//	std::ref(searchFolders),
	//	std::ref(searchContent),
	//	std::ref(vecSearchValue),
	//	std::ref(vec_folder_path),
	//	std::ref(vec_content_path),
	//	std::ref(vec_file_path));

	//thread_list.push_back(std::move(tnew));
	//current_thread_count++;




	for (int i = 1; i <= processor_count; i++)
	{
		std::thread tnew(big_for_loop,
			i,
			std::ref(processor_count),
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


	std::cin.ignore();

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


void big_for_loop(int p_i, int total_threads, double& currentfilecount, const std::filesystem::path& pathToFolder, double& percentage, double& filecount, bool& searchFolders, bool& searchContent,
	std::vector<std::string>& vecSearchValue,
	std::vector<std::filesystem::directory_entry>& vec_folder_path,
	std::vector<std::filesystem::directory_entry>& vec_content_path,
	std::vector<std::filesystem::directory_entry>& vec_file_path)
{
	int frickinFilesCounter = 0;
	int skippedFilesCounter = 0;

	bool onetime_skipped = false;
	// Iterates over every file/folder in the path of the executable and its subdiretories

	debug_log << "Started new Thread with: p_i:" << p_i << ", Total_Threads:" << total_threads << ".\n";




	//file iterator loop
	try {
		// Check if the given path exists and is a directory
		if (std::filesystem::exists(pathToFolder) && std::filesystem::is_directory(pathToFolder)) {
			for (std::filesystem::recursive_directory_iterator dirIter(pathToFolder), end; dirIter != end; ++dirIter) {
				// Print the path of the current file or directory

				const auto& entry = *dirIter;

				if (onetime_skipped)
				{
					int x = 1;
					for (; x < total_threads; x++) //onetime skip for each thread (first thread doesnt skip a file ; the 5. does skip 4 files
					{
						++dirIter;
						if (dirIter == end)
						{
							processedFiles += frickinFilesCounter;
							return;
						}
					}
					if(x>1) debug_log << "Thread " << p_i << " skipped " << x << " files at this iteration.\n";
				}
				else
				{
					int counter = 0;
					for (int i = 0; i < (p_i - 1); i++)
					{
						++dirIter;
						if (dirIter == end)
						{
							processedFiles += frickinFilesCounter;
							return;
						}
						counter++;
					}
					debug_log << "Thread Nr.:" << p_i << " skipped " << counter << " files at the start.\n";
					onetime_skipped = true;

				}

				debug_log << "Thread Nr.:" << p_i << " is processing File: \"" << entry.path().string() << "\"\n";

				frickinFilesCounter++;
				currentfilecount++;
				percentage = currentfilecount / filecount * 100;

				if (entry.is_symlink()) //skip if iterated file is a symlink
				{
					debug_log << "Thread Nr.:" << p_i << " skipped Item: \"" << entry.path().string() << "\" Reason: Symlink\n";
					continue;
				}

			}
			processedFiles += frickinFilesCounter;
		}
		else {
			std::cout << "The path does not exist or is not a directory." << std::endl;
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << e.what() << std::endl;
		processedFiles += frickinFilesCounter;
		return;
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		processedFiles += frickinFilesCounter;
		return;
	}













	//bool found = false;
	//if (std::filesystem::is_directory(dirEntry.path())) // optional: folder as iterated file
	//{
	//	if (searchFolders == false)
	//	{
	//		do_log << "Skipped Folder: \"" << dirEntry.path().string() << "\" Reason: FolderNameSearch deactivated\n";
	//		continue;
	//	}

	//	processedFolders++;
	//	do_log << "Processing Folder: \"" << dirEntry.path().string() << "\"\n";

	//	for (auto& it : vecSearchValue)
	//	{
	//		if (std::string::npos != (to_lower_string(wide_string_to_string(dirEntry.path().filename().wstring()))).find(it))
	//		{
	//			vec_folder_path.push_back(dirEntry);
	//			found = true;
	//			break;
	//		}
	//	}
	//	continue;
	//}
	//else //since its not a directory, it has to be a file
	//{
	//	do_log << "Processing File: \"" << dirEntry.path().string() << "\"\n";

	//	for (auto& it : vecSearchValue)
	//	{
	//		if (std::string::npos != (to_lower_string(wide_string_to_string(dirEntry.path().filename().wstring()))).find(it)) {
	//			vec_file_path.push_back(dirEntry);
	//			found = true;
	//			break;
	//		}
	//	}
	//	if (found || searchContent == false) //continue iterating if search was successfull on filename or content search is disabled
	//		continue;


	//	std::wifstream  inputfile;
	//	inputfile.open(dirEntry, std::wifstream::in);
	//	if (inputfile.rdstate() != std::ios_base::goodbit)
	//		continue;


	//	if (inputfile.good() && inputfile) // optional: filecontent as target
	//	{

	//		std::wstring checkedString;
	//		std::vector<std::wstring> vecOfStr; //File Content as Vector of Strings

	//		while (std::getline(inputfile, checkedString)) // Every line of the inputfile gets its own string in the vector
	//		{
	//			vecOfStr.push_back(checkedString);
	//		}
	//		inputfile.close();



	//		for (auto& it : vecOfStr) // checks every string of the vector for occurences of the wanted word and replaces it
	//		{
	//			for (auto& searchIt : vecSearchValue)
	//			{
	//				if (std::string::npos != (to_lower_string(wide_string_to_string(it))).find(searchIt))
	//				{
	//					vec_content_path.push_back(dirEntry);
	//					found = true;
	//					break;
	//				}
	//			}
	//			if (found)
	//				break;
	//		}
	//	}
	//	else
	//	{
	//		try { inputfile.close(); }
	//		catch (const std::exception&) { std::cout << "Couldnt close file!" << std::endl; }
	//	}
	//	continue;
	//}

}




