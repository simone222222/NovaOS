#include <stdint.h>
#include "IO.H"
#include "PRINT.H"
#include "PCI.H"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC


uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address =
    (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void pci_scan(void) {
    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {
                uint32_t vendor_device = pci_config_read(bus, device, function, 0x00);
                if (vendor_device == 0xFFFFFFFF)
                    continue;

                uint16_t vendor_id = vendor_device & 0xFFFF;
                uint16_t device_id = (vendor_device >> 16) & 0xFFFF;

                uint32_t class_info = pci_config_read(bus, device, function, 0x08);
                uint8_t class_code = (class_info >> 24) & 0xFF;
                uint8_t subclass   = (class_info >> 16) & 0xFF;
                uint8_t prog_if    = (class_info >> 8) & 0xFF;

                uint32_t header_type_raw = pci_config_read(bus, device, function, 0x0C);
                uint8_t header_type = (header_type_raw >> 16) & 0xFF;

                uint8_t irq_line = pci_config_read(bus, device, function, 0x3C) & 0xFF;

                // Print basic info
                printf("PCI Device %02x:%02x.%x:\n", bus, device, function);
                printf("  Vendor ID: 0x%04X, Device ID: 0x%04X\n", vendor_id, device_id);
                printf("  Class: 0x%02X, Subclass: 0x%02X, Prog IF: 0x%02X\n", class_code, subclass, prog_if);
                printf("  Header Type: 0x%02X, IRQ Line: %d\n", header_type, irq_line);

                // Print BARs
                for (int bar = 0; bar < 6; bar++) {
                    uint32_t bar_val = pci_config_read(bus, device, function, 0x10 + bar * 4);
                    printf("  BAR%d: 0x%08X\n", bar, bar_val);
                }

                printf("\n");
            }
        }
    }
}


void PCIEntry() {
    print("[PCI]: Scanning hardware devices...");
    pci_scan();
    return;
}