#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <iostream>

#pragma comment(lib, "Dbghelp.lib")

int main() {
    DWORD pid;
    std::cout << "Gib die PID des Prozesses ein, den du dumpen willst: ";
    std::cin >> pid;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (hProcess == NULL) {
        std::cout << "Failed to open process: " << GetLastError() << std::endl;
        system("pause");
        return 1;
    }

    TCHAR dumpFile[MAX_PATH];
    _stprintf_s(dumpFile, _T("dump_%u.dmp"), pid);

    HANDLE hFile = CreateFile(dumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create dump file: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        system("pause");
        return 1;
    }

    BOOL success = MiniDumpWriteDump(
        hProcess,
        pid,
        hFile,
        MiniDumpWithFullMemory,
        NULL,
        NULL,
        NULL
    );

    if (success) {
        std::cout << "Dump erfolgreich: " << dumpFile << std::endl;
    } else {
        std::cout << "Dump fehlgeschlagen: " << GetLastError() << std::endl;
    }

    CloseHandle(hFile);

    // Zielprozess beenden
    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);

    // Sich selbst beenden
    ExitProcess(0);

    return 0;
}
