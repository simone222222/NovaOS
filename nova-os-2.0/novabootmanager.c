
#include </usr/include/efi/efi.h>
#include </usr/include/efi/efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_CYAN);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"----------------------------------Nova Boot Manager--------------------------------\r\n");
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_MAGENTA);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"To boot NovaOS, Press E/e, To reset press ESC, To start terminal press T/t");
    
    while (1) {
        EFI_INPUT_KEY key;
        EFI_STATUS Status =  SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key);
        if (Status == EFI_SUCCESS) {
            if (key.ScanCode == SCAN_ESC) {
                SystemTable->BootServices->ResetSystem(EfiColdReset, EFI_SUCCESS, 0, NULL);
            }
            if (key.UnicodeChar == L'E') {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting NovaOS 2.0 in 5 seconds.");
                SystemTable->BootServices->Stall(1000000);
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting NovaOS 2.0 in 4 seconds");
                SystemTable->BootServices->Stall(1000000);
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting NovaOS 2.0 in 3 seconds");
                SystemTable->BootServices->Stall(1000000);
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting NovaOS 2.0 in 2 seconds");
                SystemTable->BootServices->Stall(1000000);
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting NovaOS 2.0 in 1 seconds");
                SystemTable->BootServices->Stall(1000000);
                // Status = LoadKernel(...);
            }
            
        }
        
    }
}
