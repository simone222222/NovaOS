//To be continued...
#include </usr/include/efi/efi.h>
#include </usr/include/efi/efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_CYAN);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"---------------------Nova Boot Manager----------------------\r\n");
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_MAGENTA);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting NovaOS 2.0")
}
