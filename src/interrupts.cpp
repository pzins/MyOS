#include "interrupts.h"


void printf(char*);
void printHex(uint8_t);


InterruptHandler::InterruptHandler(uint8_t interrupt_number_, InterruptManager* interrupt_manager_) :
interrupt_number(interrupt_number_), interrupt_manager(interrupt_manager_) {
    interrupt_manager->handlers[interrupt_number] = this;
}
InterruptHandler::~InterruptHandler() {
    if(interrupt_manager->handlers[interrupt_number] == this)
        interrupt_manager->handlers[interrupt_number] = 0;
}
uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}



InterruptManager::GateDescriptor InterruptManager::interrupt_descriptor_table[256];

InterruptManager* InterruptManager::active_interrupt_manager = 0;


void InterruptManager::SetInterruptDescriptorTableEntry(
     uint8_t interrupt_number,
     uint16_t code_segment_selector_offset,
     void (*handler)(),
     uint8_t descriptor_privilege_level,
     uint8_t descriptor_type) {

         const uint8_t IDT_DESC_PRESENT = 0x80;

         interrupt_descriptor_table[interrupt_number].handler_adress_low_bits = ((uint32_t)handler) & 0xFFFF;
         interrupt_descriptor_table[interrupt_number].handler_adress_high_bits = (((uint32_t)handler) >> 16) & 0xFFFF;
         interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector = code_segment_selector_offset;
         interrupt_descriptor_table[interrupt_number].access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privilege_level & 3) << 5);
         interrupt_descriptor_table[interrupt_number].reserved = 0;

     }

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
:pic_master_command(0x20),
pic_master_data(0x21),
pic_slave_command(0xA0),
pic_slave_data(0xA1)
{
    uint16_t code_segment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint16_t i = 0; i < 256; ++i) {
        SetInterruptDescriptorTableEntry(i, code_segment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
        handlers[i] = 0;
    }

    SetInterruptDescriptorTableEntry(0x20, code_segment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, code_segment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, code_segment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);


    pic_master_command.Write(0x11);
    pic_slave_command.Write(0x11);

    pic_master_data.Write(0x20);
    pic_slave_data.Write(0x28);


    //initialsation tell to master youre master and same slave
    pic_master_data.Write(0x04);
    pic_slave_data.Write(0x02);

    pic_master_data.Write(0x01);
    pic_slave_data.Write(0x01);

    pic_master_data.Write(0x00);
    pic_slave_data.Write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256*sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interrupt_descriptor_table;
    asm volatile("lidt %0" : : "m" (idt));

}

InterruptManager::~InterruptManager() {}

void InterruptManager::Activate() {
    if(active_interrupt_manager != 0)
        active_interrupt_manager->Deactivate();
    active_interrupt_manager = this;
    asm("sti");
}

void InterruptManager::Deactivate() {
    if(active_interrupt_manager == this)
    {
        active_interrupt_manager = 0;
        asm("cli");
    }
}



uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt_number, uint32_t esp) {
        if(active_interrupt_manager != 0) {
            return active_interrupt_manager->DoHandleInterrupt(interrupt_number, esp);
        }
        return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interrupt_number, uint32_t esp) {
    if(handlers[interrupt_number] != 0) {
        esp = handlers[interrupt_number]->HandleInterrupt(esp);
    } else if (interrupt_number != 0x20) {
         //0x20 == Timer Interrupt
         printf("UNHANDLED INTERRUPT 0x");
         printHex(interrupt_number);
     }



    //only answers the pic for hardware interrupt
    if(0x20 <= interrupt_number && interrupt_number < 0x30) {
        pic_master_command.Write(0x20);
        if(0x28 <= interrupt_number)
            pic_slave_command.Write(0x20);
    }

    return esp;
}
