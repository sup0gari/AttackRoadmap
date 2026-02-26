#include <windows.h>

BYTE* reassemble(HANDLE data_handle, DWORD* data_size) {

}

void execute(BYTE* data) {
    
}

int main() {
    HANDLE handle = CreateFileA("notepad.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) return 1;

    DWORD exe_size = 0;
    BYTE* exe = reassemble(handle, &exe_size);

    CloseHandle(handle);

    if (exe && exe_size > 0) {
        execute(exe);
        HeapFree(GetProcessHeap(), 0, exe);
    }

    return 0;
}