#include "pci.h"
#include "io.h"
#include "terminal.h"

uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    // Create configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outl(PCI_CONFIG_ADDRESS, address);
    // Read in the data
    tmp = (uint16_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

const char* get_class_name(uint8_t class_code) {
    switch(class_code) {
        case 0x01: return "Mass Storage Controller (IDE/ATA)";
        case 0x02: return "Network Controller";
        case 0x03: return "Display Controller (VGA)";
        case 0x04: return "Multimedia Controller";
        case 0x06: return "Bridge Device (PCI/ISA)";
        case 0x0C: return "Serial Bus Controller (USB)";
        default:   return "Unknown Device";
    }
}

void init_pci() {
    terminal_write("\n--- PCI BUS SCAN ---\n");
    
    // Scan all 256 buses
    for (uint16_t bus = 0; bus < 256; bus++) {
        // Scan all 32 slots (devices) on each bus
        for (uint8_t slot = 0; slot < 32; slot++) {
            
            // Read the Vendor ID (offset 0)
            uint16_t vendor_id = pci_config_read_word(bus, slot, 0, 0);
            
            // If the vendor ID is 0xFFFF, there is no device plugged into this slot!
            if (vendor_id == 0xFFFF) continue;
            
            // Read Device ID (offset 2)
            uint16_t device_id = pci_config_read_word(bus, slot, 0, 2);
            
            // Read Class and Subclass (offset 0x0A)
            uint16_t class_subclass = pci_config_read_word(bus, slot, 0, 0x0A);
            uint8_t class_code = (class_subclass >> 8) & 0xFF;
            
            terminal_write("Found: ");
            terminal_write(get_class_name(class_code));
            terminal_write("\n  Vendor: ");
            terminal_write_hex(vendor_id);
            terminal_write(" Device: ");
            terminal_write_hex(device_id);
            terminal_write("\n");
        }
    }
    terminal_write("--- SCAN COMPLETE ---\n\n");
}
