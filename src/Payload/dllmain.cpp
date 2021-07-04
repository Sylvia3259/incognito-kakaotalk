#include "pch.h"
#include "Config.h"
using namespace std;

decltype(&ShellExecuteW) lpfnShellExecuteW = ShellExecuteW;

HINSTANCE WINAPI MyShellExecuteW(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd) {
	// �ڵ� ���� ����
	static Config config;
	static wregex regex(
		LR"((?:ht|f)tps?:\/\/(?:[^@]+@)?(?:www\.)?([^:\/?]+).*)",
		wregex::icase | wregex::optimize
	);
	wsmatch matchs;
	wstring requestUrl, requestDomain;
	BOOL bOpenInIncognitoWindow = FALSE;
	browserId browser;
	wstring newFile, newParameters;

	// 1. �� �������� ���� ���� ȣ������ Ȯ��
	// 1-1. �� ������ �˻�
	if (!lpOperation || !lpFile || !lpParameters || !lpDirectory)
		goto DEFAULT_CALL;

	// 1-2. ���ڿ� �� �˻�
	if (wcscmp(lpOperation, L"open"))
		goto DEFAULT_CALL;

	if (wcscmp(lpParameters, L"") || wcscmp(lpDirectory, L""))
		goto DEFAULT_CALL;

	// 1-3. URL ���� �� ������ ����
	requestUrl = lpFile;
	if (!regex_match(requestUrl, matchs, regex))
		goto DEFAULT_CALL;

	if (matchs.size() > 1)
		requestDomain = matchs[1];

	// 2. ����ڰ� ��ũ�� â�� ���� ���� ������ ���ߴ��� �˻�
	for (const auto action : config.GetActions()) {
		switch (action) {
		case actionId::LCLS:
			// 2-1. ���� ��Ʈ�� + ���� ����Ʈ
			if (GetAsyncKeyState(VK_LCONTROL) < 0 && GetAsyncKeyState(VK_LSHIFT) < 0)
				bOpenInIncognitoWindow = TRUE;
			break;
		case actionId::LC:
			// 2-2. ���� ��Ʈ��
			if (GetAsyncKeyState(VK_LCONTROL) < 0 && GetAsyncKeyState(VK_LSHIFT) >= 0)
				bOpenInIncognitoWindow = TRUE;
			break;
		}
	}

	// 3. ������ ���� ���� �˻�
	// 3-1. ����ڰ� �ش� �������� ���� ��ũ�� â���� ���� �ʵ��� �����Ͽ����� �˻�
	if (bOpenInIncognitoWindow)
		for (const auto& domain : config.GetDomains("never_open_in_incognito_window"))
			if (domain == requestDomain)
				bOpenInIncognitoWindow = FALSE;

	// 3-2. ����ڰ� �ش� �������� �׻� ��ũ�� â���� ������ �����Ͽ����� �˻�
	if (!bOpenInIncognitoWindow)
		for (const auto& domain : config.GetDomains("always_open_in_incognito_window"))
			if (domain == requestDomain)
				bOpenInIncognitoWindow = TRUE;

	if (!bOpenInIncognitoWindow)
		goto DEFAULT_CALL;

	// 4. ������ ����
	browser = config.GetBrowser();
	if (browser == browserId::DEFAULT)
		browser = config.GetDefaultBrowser();

	switch (browser) {
	case browserId::CHROME:
		newFile = L"chrome";
		newParameters = wstring(lpFile) + L" -incognito";
		break;
	case browserId::UNKNOWN:
	case browserId::EDGE:
		newFile = L"msedge";
		newParameters = wstring(lpFile) + L" -inprivate";
		break;
	case browserId::FIREFOX:
		newFile = L"firefox";
		newParameters = L"-private-window " + wstring(lpFile);
		break;
	case browserId::WHALE:
		newFile = L"whale";
		newParameters = wstring(lpFile) + L" -incognito";
		break;
	}

	if (newFile.empty() || newParameters.empty())
		goto DEFAULT_CALL;

	// 5. ��ũ�� â ����
	return lpfnShellExecuteW(hwnd, lpOperation, newFile.c_str(), newParameters.c_str(), lpDirectory, nShowCmd);

DEFAULT_CALL:
	return lpfnShellExecuteW(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	TCHAR szProcessName[MAX_PATH];

	GetModuleFileName(NULL, szProcessName, MAX_PATH);
	if (_tcsstr(szProcessName, _T("\\KakaoTalk.exe"))) {
		if (DetourIsHelperProcess())
			return TRUE;

		if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
			DetourRestoreAfterWith();
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)lpfnShellExecuteW, MyShellExecuteW);
			DetourTransactionCommit();
		}
		else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)lpfnShellExecuteW, MyShellExecuteW);
			DetourTransactionCommit();
		}
	}

	return TRUE;
}