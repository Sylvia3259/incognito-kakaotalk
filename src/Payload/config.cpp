#include "pch.h"
#include "config.h"

#pragma comment(linker, "/SECTION:.shared,RWS")
#pragma data_seg(".shared")
nlohmann::json config;
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

_declspec(dllexport) void SetConfig(std::string configFilePath) {
	std::ifstream configFile(configFilePath);
	configFile >> config;
}

_declspec(dllexport) const nlohmann::json& GetConfig() {
	return config;
}

browser GetBrowser() {
	const nlohmann::json& browserConfig = config["browser"];

	if (browserConfig.is_string()) {
		const auto browserId = browsers.find(browserConfig.get<std::string>());
		if (browserId != browsers.end())
			return browserId->second;
	}

	return browser::DEFAULT;
}

std::set<action> GetActions() {
	const nlohmann::json& actionsConfig = config["actions"];
	std::set<action> actionIds;

	if (actionsConfig.is_array()) {
		for (const auto& actionConfig : actionsConfig) {
			if (actionConfig.is_string()) {
				const auto actionId = actions.find(actionConfig.get<std::string>());
				if (actionId != actions.end())
					actionIds.insert(actionId->second);
			}
			else {
				actionIds.clear();
				break;
			}
		}
	}

	if (actionIds.empty())
		actionIds.insert(action::LCLS);

	return actionIds;
}

std::vector<std::string> GetDomains(std::string type) {
	const nlohmann::json& domainsConfig = config["domains"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	if (domainsConfig.is_array() && domainsConfig.size() == 1) {
		const nlohmann::json& domains = ((nlohmann::json&)domainsConfig[0])[type];
		if (domains.is_array()) {
			if (all_of(domains.begin(), domains.end(), isString))
				return domains.get<std::vector<std::string>>();
		}
	}

	return std::vector<std::string>();
}