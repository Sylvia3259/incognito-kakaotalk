#include "pch.h"
using namespace std;
using json = nlohmann::json;

#pragma comment(linker, "/SECTION:.shared,RWS")
#pragma data_seg(".shared")
json config;
#pragma data_seg()

decltype(&ShellExecuteW) lpfnShellExecuteW = ShellExecuteW;

HINSTANCE WINAPI MyShellExecuteW(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd) {
	WCHAR* szArgument = NULL;
	DWORD dwArgumentLength;
	HINSTANCE hResult = NULL;

	if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) && (GetAsyncKeyState(VK_LSHIFT) & 0x8000)) {
		if (lpFile) {
			dwArgumentLength = wcslen(lpFile) + 13;
			szArgument = (WCHAR*)malloc(sizeof(WCHAR) * dwArgumentLength);
			if (szArgument) {
				if (swprintf_s(szArgument, dwArgumentLength, L"%s --incognito", lpFile) >= 0)
					hResult = lpfnShellExecuteW(hwnd, lpOperation, L"chrome", szArgument, lpDirectory, nShowCmd);
				free(szArgument);
			}
		}
	}

	if (!hResult)
		hResult = lpfnShellExecuteW(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

	return hResult;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	TCHAR szProcessName[MAX_PATH];

	GetModuleFileName(NULL, szProcessName, MAX_PATH);
	if (_tcsstr(szProcessName, _T("KakaoTalk.exe"))) {
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

_declspec(dllexport) void Initialize(string configFilePath) {
	std::ifstream configFile(configFilePath);
	configFile >> config;
}