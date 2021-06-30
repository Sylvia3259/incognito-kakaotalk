#pragma once
#include <string>
#include <Windows.h>

inline std::string RelativePathA(const std::string& path) {
	CHAR buffer[MAX_PATH] = {};
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	size_t position;
	std::string currentDirectory = buffer;
	position = currentDirectory.find_last_of("\\/");
	currentDirectory = currentDirectory.substr(0, position + 1);

	return currentDirectory + path;
}

inline std::wstring RelativePathW(const std::wstring& path) {
	WCHAR buffer[MAX_PATH] = {};
	GetModuleFileNameW(NULL, buffer, MAX_PATH);

	size_t position;
	std::wstring currentDirectory = buffer;
	position = currentDirectory.find_last_of(L"\\/");
	currentDirectory = currentDirectory.substr(0, position + 1);

	return currentDirectory + path;
}

#ifdef UNICODE
#define RelativePath  RelativePathW
#else
#define RelativePath  RelativePathA
#endif // !UNICODE