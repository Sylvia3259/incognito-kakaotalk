#include <tchar.h>
#include <Windows.h>
#include <BlackBone/Process/Process.h>
#include "relative.h"
using namespace std;
using namespace blackbone;

BOOL Inject(DWORD dwProcessId) {
	Process process;
	NTSTATUS status;

	status = process.Attach(dwProcessId);
	if (!NT_SUCCESS(status))
		return FALSE;

	auto result = process.modules().Inject(RelativePathW(L"Payload.dll"));
	if (!result)
		return FALSE;

	status = process.Detach();
	if (!NT_SUCCESS(status))
		return FALSE;

	return TRUE;
}

int main() {
	Process process;
	DWORD processId = 0;
	NTSTATUS status;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	for (;;) {
		if (processId) {
			if (process.valid()) {
				process.modules().reset();
				auto module = process.modules().GetModule(L"Payload.dll");
				if (!module)
					Inject(processId);
			}
			else {
				process.Detach();
				processId = 0;
			}
		}
		else {
			status = process.Attach(L"KakaoTalk.exe");
			if (NT_SUCCESS(status))
				processId = process.core().pid();
		}

		Sleep(1000);
	}

	return 0;
}