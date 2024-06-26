// Includes
#include "gdt.h"

// Initialize an array of GDT entries and a GDT pointer
struct gdt_entry gdt[3];
struct gdt_ptr   gp;

// Get function from the assembly file
extern void gdt_flush();

// Sets a gate of a GDT entry using the specified arguments
void gdt_set_gate(int32_t num, uint64_t base, uint64_t limit, uint8_t access,
                  uint8_t granularity) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= (granularity & 0xF0);
    gdt[num].access = access;
}

// Sets up the GDT
void gdt_install() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base  = (uintptr_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush();
}
