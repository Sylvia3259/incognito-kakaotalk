#pragma once
#include <string>
#include <Windows.h>

inline std::string RelativePathA(const std::string& path) {
	CHAR szFilePath[MAX_PATH];
	size_t position;

	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);

	std::string currentDirectory = szFilePath;
	position = currentDirectory.find_last_of("\\/");
	currentDirectory = currentDirectory.substr(0, position + 1);

	return currentDirectory + path;
}

inline std::wstring RelativePathW(const std::wstring& path) {
	WCHAR szFilePath[MAX_PATH];
	size_t position;

	GetModuleFileNameW(NULL, szFilePath, MAX_PATH);

	std::wstring currentDirectory = szFilePath;
	position = currentDirectory.find_last_of(L"\\/");
	currentDirectory = currentDirectory.substr(0, position + 1);

	return currentDirectory + path;
}

#ifdef UNICODE
#define RelativePath  RelativePathW
#else
#define RelativePath  RelativePathA
#endif // !UNICODE