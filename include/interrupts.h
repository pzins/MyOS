#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H
#include "gdt.h"
#include "types.h"
#include "port.h"

class InterruptManager;


class InterruptHandler
{
protected:
    uint8_t interrupt_number;
    InterruptManager* interrupt_manager;

    InterruptHandler(uint8_t interrupt_number_, InterruptManager* Interrupt_manager_);
    ~InterruptHandler();

public:
    virtual uint32_t HandleInterrupt(uint32_t esp);

};


class InterruptManager
{
    friend class InterruptHandler;

protected:

    static InterruptManager* active_interrupt_manager;
    InterruptHandler* handlers[256];
    struct GateDescriptor
    {
        uint16_t handler_adress_low_bits;
        uint16_t gdt_code_segment_selector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handler_adress_high_bits;
    } __attribute__((packed));

    static GateDescriptor interrupt_descriptor_table[256];

    struct InterruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    uint16_t hardware_interrupt_offset;

    static void SetInterruptDescriptorTableEntry(
        uint8_t interrupt_number,
        uint16_t code_segment_selector_offset,
        void (*handler)(),
        uint8_t descriptor_privilege_level,
        uint8_t descriptor_type);


        Port8bitSlow pic_master_command;
        Port8bitSlow pic_master_data;
        Port8bitSlow pic_slave_command;
        Port8bitSlow pic_slave_data;


public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();

    static uint32_t HandleInterrupt(uint8_t interrupt_number, uint32_t esp);
    uint32_t DoHandleInterrupt(uint8_t interrupt_number, uint32_t esp);

    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x01();  //keyboard int
    static void HandleInterruptRequest0x00();  //timer int
    static void HandleInterruptRequest0x0C();  //mouse int

    void Activate();
    void Deactivate();

};




#endif
