#include </usr/include/efi/efi.h>
#include </usr/include/efi/efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable SystemTable->ConOut->OutputString(SystemTable->ConOut, L"-------------------Nova Boot Manager----------------------");
SystemTable->ConOut->OutputString(SystemTable->ConOut, L"")
}