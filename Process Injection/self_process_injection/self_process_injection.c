#include <windows.h>

BYTE* read_file(HANDLE data_handle, DWORD* data_size) {
    *data_size = GetFileSize(data_handle, NULL);
    if (*data_size == INVALID_FILE_SIZE) return NULL;

    BYTE* buffer = (BYTE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, *data_size);
    if (buffer == NULL) return NULL;

    DWORD bytes_read;
    if (!ReadFile(data_handle, buffer, *data_size, &bytes_read, NULL) || bytes_read != *data_size) {
        HeapFree(GetProcessHeap(), 0, buffer);
        return NULL;
    }

    return buffer;
}

void execute(BYTE* data) {
    // Check magic number
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)data;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return;

    // Check data size
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(data + dos->e_lfanew);
    PVOID base = VirtualAlloc(NULL, nt->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!base) return;

    // Copy section table (DOS header, NT header, Section table)
    memcpy(base, data, nt->OptionalHeader.SizeOfHeaders);
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt);

    // Copy section data
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        memcpy((BYTE*)base + section[i].VirtualAddress, (BYTE*)data + section[i].PointerToRawData, section[i].SizeOfRawData);
    }
}

int main() {
    HANDLE handle = CreateFileA("notepad.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) return 1;

    DWORD exe_size = 0;
    BYTE* exe = read_file(handle, &exe_size);

    CloseHandle(handle);

    if (exe && exe_size > 0) {
        execute(exe);
        HeapFree(GetProcessHeap(), 0, exe);
    }

    return 0;
}