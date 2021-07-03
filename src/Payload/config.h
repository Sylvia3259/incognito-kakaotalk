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

_declspec(dllexport) const nlohmann::json& GetConfig();
_declspec(dllexport) bool SetConfig(std::string configFilePath);
_declspec(dllexport) void SetConfig(const nlohmann::json& newConfig);

class Config {
public:
	Config();

private:
	void ParseBrowser();
	void ParseActions();
	void ParseDomains(std::string type);

	nlohmann::json config;

	browser browserId;
	std::set<action> actionIds;
	std::map<std::string, std::vector<std::string>> domains;
};