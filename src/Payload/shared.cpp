#include "pch.h"
#include "shared.h"

#pragma comment(linker, "/SECTION:.shared,RWS")
#pragma data_seg(".shared")
CHAR configFilePath[MAX_PATH] = "";
#pragma data_seg()

_declspec(dllexport) std::string GetConfigFilePath() {
	return configFilePath;
}

_declspec(dllexport) bool SetConfigFilePath(std::string newConfigFilePath) {
	return !strcpy_s(configFilePath, newConfigFilePath.c_str());
}