#pragma once
#include "wholeinclude.h"


void printProgress(double& percentage);

bool validateInputStringForInitialInput(std::string input, std::vector<std::string>& output);


std::string getFolderPath(std::string pathOfFile);


void BrowseToFile(LPCWSTR filename);


//thx to "Jonathan Potter" on stackoverflow;
void BrowseToFolder(LPCWSTR filename);


//validates inputstring (returns false if wrong input) and transfers input string to vector
bool validateInputStringForOpening(std::string input, std::vector<int>& output);


void display(
	int processedFiles,
	int processedFolders,
	auto seconds,
	std::vector<std::filesystem::directory_entry>&vec_folder_path,
	std::vector<std::filesystem::directory_entry>&vec_content_path,
	std::vector<std::filesystem::directory_entry>&vec_file_path);


void big_for_loop(int p_i, int total_threads, double& currentfilecount, const std::filesystem::path& pathToFolder, double& percentage, double& filecount, bool& searchFolders, bool& searchContent,
	std::vector<std::string>& vecSearchValue,
	std::vector<std::filesystem::directory_entry>& vec_folder_path,
	std::vector<std::filesystem::directory_entry>& vec_content_path,
	std::vector<std::filesystem::directory_entry>& vec_file_path);


void startWinXSearch(const std::filesystem::path pathToFolder, bool searchFolders, bool searchContent, std::vector<std::string> vecSearchValue);
