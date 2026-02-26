#include <windows.h>

BYTE* convert(HANDLE data_handle, DWORD* data_size) {
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

}

int main() {
    HANDLE handle = CreateFileA("notepad.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) return 1;

    DWORD exe_size = 0;
    BYTE* exe = convert(handle, &exe_size);

    CloseHandle(handle);

    if (exe && exe_size > 0) {
        execute(exe);
        HeapFree(GetProcessHeap(), 0, exe);
    }

    return 0;
}