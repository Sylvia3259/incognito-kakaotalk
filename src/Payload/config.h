#pragma once
#include "pch.h"

const enum browser {
	DEFAULT,
	CHROME,
	EDGE,
	FIREFOX,
	WHALE,
};

const enum action {
	LCLS,
};

_declspec(dllexport) void SetConfig(std::string configFilePath);
_declspec(dllexport) const nlohmann::json& GetConfig();

browser GetBrowser();
std::set<action> GetActions();
std::vector<std::string> GetDomains(std::string type);