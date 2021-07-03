#include "pch.h"
#include "ConfigManager.h"

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

ConfigManager::ConfigManager() {
	ParseConfig(GetConfigFilePath());

	ParseBrowser(browser::DEFAULT);
	ParseActions(action::LCLS);
	ParseDomains("always_open_in_incognito_window");
	ParseDomains("never_open_in_incognito_window");
}

void ConfigManager::ParseConfig(std::string configFilePath) {
	try {
		std::ifstream configFile(configFilePath);
		configFile >> config;
	}
	catch (...) {
		config = nlohmann::json();
	}
}

void ConfigManager::ParseBrowser(browser fallback) {
	const nlohmann::json& browserConfig = this->config["browser"];

	if (browserConfig.is_string()) {
		const auto iterator = browsers.find(browserConfig.get<std::string>());
		if (iterator != browsers.end()) {
			this->browserId = iterator->second;
			return;
		}
	}

	this->browserId = fallback;
}

void ConfigManager::ParseActions(action fallback) {
	const nlohmann::json& actionsConfig = this->config["actions"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	if (actionsConfig.is_array()) {
		if (all_of(actionsConfig.begin(), actionsConfig.end(), isString)) {
			for (const auto& action : actionsConfig) {
				const auto iterator = actions.find(action.get<std::string>());
				if (iterator != actions.end())
					this->actionIds.insert(iterator->second);
			}
		}
	}

	if (this->actionIds.empty())
		this->actionIds.insert(fallback);
}

void ConfigManager::ParseDomains(std::string type) {
	const nlohmann::json& domainsConfig = this->config["domains"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	this->domains[type] = std::vector<std::string>();

	if (domainsConfig.is_array() && domainsConfig.size() == 1) {
		const nlohmann::json& domains = ((nlohmann::json&)domainsConfig[0])[type];
		if (domains.is_array()) {
			if (all_of(domains.begin(), domains.end(), isString))
				this->domains[type] = domains.get<std::vector<std::string>>();
		}
	}
}