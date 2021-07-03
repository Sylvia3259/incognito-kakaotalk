#include "pch.h"
#include "Config.h"

const std::map<std::string, browserId> browserTable = {
	{ "default", browserId::DEFAULT },
	{ "chrome", browserId::CHROME },
	{ "edge", browserId::EDGE },
	{ "firefox", browserId::FIREFOX },
	{ "whale", browserId::WHALE },
};

const std::map<std::string, actionId> actionTable = {
	{ "LCLS", actionId::LCLS },
};

Config::Config() {
	this->ParseConfig(GetConfigFilePath());

	this->ParseBrowser(browserId::DEFAULT);
	this->ParseActions(actionId::LCLS);
	this->ParseDomains("always_open_in_incognito_window");
	this->ParseDomains("never_open_in_incognito_window");
}

const nlohmann::json& Config::GetConfig() const noexcept {
	return this->config;
}

const browserId Config::GetBrowser() const noexcept {
	return this->browser;
}

const std::set<actionId>& Config::GetActions() const noexcept {
	return this->actions;
}

const std::vector<std::wstring>& Config::GetDomains(std::string type) const noexcept {
	const auto iterator = this->domains.find(type);
	if (iterator != this->domains.end())
		return iterator->second;

	return std::vector<std::wstring>();
}

void Config::ParseConfig(std::string configFilePath) {
	try {
		std::ifstream configFile(configFilePath);
		configFile >> this->config;
	}
	catch (...) {
		this->config = nlohmann::json();
	}
}

void Config::ParseBrowser(browserId fallback) {
	const nlohmann::json& browserConfig = this->config["browser"];

	if (browserConfig.is_string()) {
		const auto iterator = browserTable.find(browserConfig.get<std::string>());
		if (iterator != browserTable.end()) {
			this->browser = iterator->second;
			return;
		}
	}

	this->browser = fallback;
}

void Config::ParseActions(actionId fallback) {
	const nlohmann::json& actionsConfig = this->config["actions"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	if (actionsConfig.is_array()) {
		if (all_of(actionsConfig.begin(), actionsConfig.end(), isString)) {
			for (const auto& action : actionsConfig) {
				const auto iterator = actionTable.find(action.get<std::string>());
				if (iterator != actionTable.end())
					this->actions.insert(iterator->second);
			}
		}
	}

	if (this->actions.empty())
		this->actions.insert(fallback);
}

void Config::ParseDomains(std::string type) {
	const nlohmann::json& domainsConfig = this->config["domains"];

	const auto isString = [](const nlohmann::json& j) {
		return j.is_string();
	};

	const auto convertToWstring = [](const std::string& s) {
		WCHAR szBuffer[2048] = {};
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), szBuffer, 2048);
		return std::wstring(szBuffer);
	};

	this->domains[type] = std::vector<std::wstring>();

	if (domainsConfig.is_array() && domainsConfig.size() == 1) {
		const nlohmann::json& domains = ((nlohmann::json&)domainsConfig[0])[type];
		if (domains.is_array()) {
			if (all_of(domains.begin(), domains.end(), isString))
				for (const auto& domain : domains.get<std::vector<std::string>>())
					this->domains[type].push_back(convertToWstring(domain));
		}
	}
}