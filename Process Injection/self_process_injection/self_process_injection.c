#include <windows.h>
#include <stdio.h>

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

PVOID relocate(BYTE* data) {
    // Check magic number
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)data;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return NULL;

    // Check data size
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(data + dos->e_lfanew);
    PVOID base = VirtualAlloc(NULL, nt->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!base) return NULL;

    // Copy section table (DOS header, NT header, Section table)
    memcpy(base, data, nt->OptionalHeader.SizeOfHeaders);
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt);

    // Copy section data
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        memcpy((BYTE*)base + section[i].VirtualAddress, (BYTE*)data + section[i].PointerToRawData, section[i].SizeOfRawData);
    }

    // IAT
    PIMAGE_DATA_DIRECTORY import_dir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (import_dir->Size > 0) {
        // DLL list
        PIMAGE_IMPORT_DESCRIPTOR import_desc_list = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)base + import_dir->VirtualAddress);
        while (import_desc_list->Name) {
            HMODULE dll_handle = LoadLibraryA((char*)((BYTE*)base + import_desc_list->Name));
            if (dll_handle) {
                PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((BYTE*)base + import_desc_list->FirstThunk);
                PIMAGE_THUNK_DATA original_thunk = (PIMAGE_THUNK_DATA)((BYTE*)base + import_desc_list->OriginalFirstThunk);
                while (original_thunk->u1.AddressOfData) {
                    if (IMAGE_SNAP_BY_ORDINAL(original_thunk->u1.Ordinal)) {
                        thunk->u1.Function = (ULONG_PTR)GetProcAddress(dll_handle, (LPCSTR)IMAGE_ORDINAL(original_thunk->u1.Ordinal));
                    } else {
                        PIMAGE_IMPORT_BY_NAME function = (PIMAGE_IMPORT_BY_NAME)((BYTE*)base + original_thunk->u1.AddressOfData);
                        thunk->u1.Function = (ULONG_PTR)GetProcAddress(dll_handle, function->Name);
                    }
                    thunk++;
                    original_thunk++;
                }
            }
            import_desc_list++;
        }
    }

    // Base relocation
    ULONG_PTR delta = (ULONG_PTR)base - nt->OptionalHeader.ImageBase;
    if (delta != 0) {
        PIMAGE_DATA_DIRECTORY data_dir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
        if (data_dir->Size > 0) {
            PIMAGE_BASE_RELOCATION base_reloc = (PIMAGE_BASE_RELOCATION)((BYTE*)base + data_dir->VirtualAddress);
            while (base_reloc->SizeOfBlock > 0) {
                DWORD calc_count = (base_reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION) / sizeof(WORD));
                WORD* list = (WORD*)(base_reloc + 1);
                for (DWORD i = 0; i < calc_count; i++) {
                    if ((list[i] >> 12) != IMAGE_REL_BASED_ABSOLUTE) {
                        *(ULONG_PTR*)((BYTE*)base + base_reloc->VirtualAddress + (list[i] & 0xFFF)) += delta;
                    }
                }
                base_reloc = (PIMAGE_BASE_RELOCATION)((BYTE*)base_reloc + base_reloc->SizeOfBlock);
            }
        }
    }

    return base;
}

int main() {
    HANDLE handle = CreateFileA("target.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) return 1;

    DWORD exe_size = 0;
    BYTE* exe = read_file(handle, &exe_size);

    CloseHandle(handle);

    if (exe && exe_size > 0) {
        PVOID base = relocate(exe);
        if (base) {
            PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
            PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)base + dos->e_lfanew);

            typedef void (*EntryPoint_t)();
            EntryPoint_t entry_point = (EntryPoint_t)((BYTE*)base + nt->OptionalHeader.AddressOfEntryPoint);
            
            printf("Jumping to EntryPoint: %p\n", entry_point);
            entry_point();
        }
        HeapFree(GetProcessHeap(), 0, exe);
    }

    return 0;
}