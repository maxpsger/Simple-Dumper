#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <iostream>

#pragma comment(lib, "Dbghelp.lib")

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: dumper.exe <PID>" << std::endl;
        return 1;
    }

    DWORD pid = atoi(argv[1]);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (hProcess == NULL) {
        std::cout << "Failed to open process: " << GetLastError() << std::endl;
        return 1;
    }
    TCHAR dumpFile[MAX_PATH];
    _stprintf_s(dumpFile, _T("dump_%u.dmp"), pid);

    HANDLE hFile = CreateFile(dumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create dump file: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    BOOL sucess = MiniDumpWriteDump(
        hProcess,
        pid,
        hFile,
        MiniDumpWithFullMemory,
        NULL,
        NULL,
        NULL
    );

    if (sucess) {
        std::cout << "Dump sucessful: " << dumpFile << std::endl;
    }
    else {
        std::cout << "Dump failed" << GetLastError() << std::endl;
    }

    CloseHandle(hFile);

    //Ends Process immidiatly
    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);

    //ends the dumper itself
    ExitProcess(0);

    return 0;
}