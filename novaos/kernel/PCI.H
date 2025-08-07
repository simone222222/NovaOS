#ifndef _PCI_H
#define _PCI_H
#include <stdint.h>

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
void pci_scan(void);
void PCIEntry();

#endif