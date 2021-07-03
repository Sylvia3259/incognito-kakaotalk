#pragma once
#include "pch.h"

_declspec(dllexport) std::string GetConfigFilePath();
_declspec(dllexport) bool SetConfigFilePath(std::string newConfigFilePath);