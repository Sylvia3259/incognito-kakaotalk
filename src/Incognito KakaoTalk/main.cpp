#include <string>
#include <tchar.h>
#include <Windows.h>
#include <BlackBone/Process/Process.h>
#include "relative.h"
using namespace std;
using namespace blackbone;

int main() {
	const WCHAR szTarget[] = L"KakaoTalk.exe";
	const wstring payload = RelativePathW(L"Payload.dll");

	Process process;
	DWORD dwProcessId = 0;
	NTSTATUS status;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	for (;;) {
		if (dwProcessId) {
			if (process.valid()) {
				process.modules().reset();
				auto module = process.modules().GetModule(payload);
				if (!module)
					process.modules().Inject(payload);
			}
			else {
				process.Detach();
				dwProcessId = 0;
			}
		}
		else {
			status = process.Attach(szTarget);
			if (NT_SUCCESS(status))
				dwProcessId = process.core().pid();
		}

		Sleep(1000);
	}

	return 0;
}