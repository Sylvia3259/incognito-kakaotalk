#pragma once
#include "pch.h"
#include "shared.h"

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

class ConfigManager {
public:
	ConfigManager();

private:
	void ParseConfig(std::string configFilePath);

	void ParseBrowser(browser fallback);
	void ParseActions(action fallback);
	void ParseDomains(std::string type);

	nlohmann::json config;

	browser browserId;
	std::set<action> actionIds;
	std::map<std::string, std::vector<std::string>> domains;
};