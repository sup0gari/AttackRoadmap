#include <windows.h>
#include <stdio.h>

void get_api_address(const char* dll_name, const char* api_name) {
    HMODULE handle_amsi = LoadLibraryA(dll_name);
    if (!handle_amsi) {
        printf("[-] Loading amsi.dll failed.\n");
        return;
    }

    void* ptr_amsi_scan_buffer = GetProcAddress(handle_amsi, api_name);
    if (!ptr_amsi_scan_buffer) {
        printf("[-] AmsiScanBuffer address not found.\n");
        FreeLibrary(handle_amsi);
        return;
    }

    printf("[+] amsi.dll base address:  0x%p\n", handle_amsi);
    printf("[+] AmsiScanBuffer address: 0x%p\n", ptr_amsi_scan_buffer);

    ULONG_PTR offset = (ULONG_PTR)ptr_amsi_scan_buffer - (ULONG_PTR)handle_amsi;
    printf("[!] Offset: 0x%llX\n", (unsigned long long)offset);

    FreeLibrary(handle_amsi);
}

int main() {
    get_api_address("amsi.dll", "AmsiScanBuffer");
    return 0;
}