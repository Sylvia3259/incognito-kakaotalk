#pragma once
#include "pch.h"
#include "shared.h"

const enum browserId {
	DEFAULT,
	CHROME,
	EDGE,
	FIREFOX,
	WHALE,
	UNKNOWN,
};

const enum actionId {
	LCLS,
	LC,
};

class Config {
public:
	Config();

	const nlohmann::json& GetConfig() const noexcept;

	const browserId GetBrowser() const noexcept;
	const std::set<actionId>& GetActions() const noexcept;
	const std::vector<std::wstring>& GetDomains(std::string type) const noexcept;

	const browserId GetDefaultBrowser() const;

private:
	void ParseConfig(std::string configFilePath);

	void ParseBrowser(browserId fallback);
	void ParseActions(actionId fallback);
	void ParseDomains(std::string type);

	nlohmann::json config;

	browserId browser;
	std::set<actionId> actions;
	std::map<std::string, std::vector<std::wstring>> domains;
};