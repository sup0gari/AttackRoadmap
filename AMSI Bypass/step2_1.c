#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>

void enumerate_modules(DWORD pid) {
    HANDLE handle_snapshot;
    MODULEENTRY32 me32;

    handle_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (handle_snapshot == INVALID_HANDLE_VALUE) {
        printf("Snapshot failed. Error: %lu\n", GetLastError());
        return;
    }

    me32.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(handle_snapshot, &me32)) {
        printf("Module information not found.\n");
        CloseHandle(handle_snapshot);
        return;
    }

    printf("%-30s %-20s\n", "Module Name", "Base Address");
    printf("------------------------------------------------------------\n");

    do {
        printf("%-30s 0x%p\n", me32.szModule, me32.modBaseAddr);
    } while (Module32Next(handle_snapshot, &me32));

    CloseHandle(handle_snapshot);
}

int main() {
    DWORD pid;

    printf("Target PID: ");
    if (scanf("%lu", &pid) != 1) {
        printf("Invalid PID.\n");
        return 1;
    }

    enumerate_modules(pid);

    printf("\nDone. Exiting.\n");
    return 0;
}