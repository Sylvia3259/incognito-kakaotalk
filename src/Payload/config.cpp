#include "pch.h"
#include "config.h"

#pragma comment(linker, "/SECTION:.shared,RWS")
#pragma data_seg(".shared")
nlohmann::json sharedConfig;
#pragma data_seg()

const std::map<std::string, browser> browsers = {
	{ "default", browser::DEFAULT },
	{ "chrome", browser::CHROME },
	{ "edge", browser::EDGE },
	{ "firefox", browser::FIREFOX },
	{ "whale", browser::WHALE },
};

const std::map<std::string, action> actions = {
	{ "LCLS", action::LCLS },
};

_declspec(dllexport) const nlohmann::json& GetConfig() {
	return sharedConfig;
}

_declspec(dllexport) bool SetConfig(std::string configFilePath) {
	try {
		std::ifstream configFile(configFilePath);
		configFile >> sharedConfig;
		return true;
	}
	catch (...) {
		return false;
	}
}

_declspec(dllexport) void SetConfig(const nlohmann::json& newConfig) {
	sharedConfig = newConfig;
}

Config::Config() {
	config = GetConfig();

	browserId = browser::DEFAULT;
	ParseBrowser();

	ParseActions();
	if (actionIds.empty())
		actionIds.insert(action::LCLS);

	domains["always_open_in_incognito_window"];
	ParseDomains("always_open_in_incognito_window");

	domains["never_open_in_incognito_window"];
	ParseDomains("never_open_in_incognito_window");
}

void Config::ParseBrowser() {
	const nlohmann::json& browserConfig = config["browser"];

	if (browserConfig.is_string()) {
		const auto iterator = browsers.find(browserConfig.get<std::string>());
		if (iterator != browsers.end())
			browserId = iterator->second;
	}
}

void Config::ParseActions() {
	const nlohmann::json& actionsConfig = config["actions"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	if (actionsConfig.is_array()) {
		if (all_of(actionsConfig.begin(), actionsConfig.end(), isString)) {
			for (const auto& action : actionsConfig) {
				const auto iterator = actions.find(action.get<std::string>());
				if (iterator != actions.end())
					actionIds.insert(iterator->second);
			}
		}
	}
}

void Config::ParseDomains(std::string type) {
	const nlohmann::json& domainsConfig = config["domains"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	if (domainsConfig.is_array() && domainsConfig.size() == 1) {
		const nlohmann::json& domains = ((nlohmann::json&)domainsConfig[0])[type];
		if (domains.is_array()) {
			if (all_of(domains.begin(), domains.end(), isString))
				this->domains[type] = domains.get<std::vector<std::string>>();
		}
	}
}