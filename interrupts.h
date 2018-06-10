#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

// IDTR Register (see IA32-3A, Â§6.10 INTERRUPT DESCRIPTOR TABLE).
// IDT puede albergar 256 descriptors como maximo.
// Cada descriptor de la IDT tiene un tamaño fijo de 8 bytes
// El IDT base addres debe estar alineada a 8 bytes.
// El valor del limite debe ser siempre (8*N - 1)
// El registro de la IDT (IDTR) contiene la informacion de la IDT
struct IDTR {
    uint16_t limit;  // Limit: tiene tamaño 16 bits
    uint32_t base;   // Base address: tiene tamaño 32 bits
} __attribute__((packed));


// Gate descriptors for interrupts (see IA32-3A, Â§6.11 IDT DESCRIPTORS).
struct Gate {
    unsigned off_15_0 : 16;   // Low 16 bits of offset in segment.
    unsigned segment : 16;    // Segment selector (always KSEG_CODE).
    unsigned reserved1 : 8;   // Unused/reserved.
    unsigned type : 4;        // Type (always STS_IG32).
    unsigned system : 1;      // System bit (must be 0).
    unsigned rpl : 2;         // Requestor Privilege Level (always 0).
    unsigned present : 1;     // Present (must be 1 if active).
    unsigned off_31_16 : 16;  // High bits of offset in segment.
};


// x86 exception numbers (see IA32-3A, Â§6.3 SOURCES OF INTERRUPTS).
enum Exception {
    T_DIVIDE = 0,    // Divide error
    T_DEBUG = 1,     // Debug exception
    T_NMI = 2,       // Non-maskable interrupt
    T_BRKPT = 3,     // Breakpoint
    T_OFLOW = 4,     // Overflow
    T_BOUND = 5,     // Bounds check
    T_ILLOP = 6,     // Illegal opcode
    T_DEVICE = 7,    // Device not available
    T_DBLFLT = 8,    // Double fault
    /* T_COPROC */   // Reserved (not generated by recent processors)
    T_TSS = 10,      // Invalid task switch segment
    T_SEGNP = 11,    // Segment not present
    T_STACK = 12,    // Stack exception
    T_GPFLT = 13,    // General protection fault
    T_PGFLT = 14,    // Page fault
    /* T_RES */      // Reserved
    T_FPERR = 16,    // Floating point error
    T_ALIGN = 17,    // Aligment check
    T_MCHK = 18,     // Machine check
    T_SIMDERR = 19,  // SIMD floating point error
};

// kern2 interrupt numbers: we map IRQ0 to 32, and count from there.
enum Interrupt {
    T_TIMER = 32,     // IRQ0
    T_KEYBOARD = 33,  // IRQ1
};

// interrupts.c
void idt_init(void);
void idt_install(uint8_t n, void (*handler)(void));

// idt_entry.S
void breakpoint(void);

#endif
