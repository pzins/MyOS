#ifndef __MYOS__HARWAREDCOMMUNICATION__INTERRUPTMANAGER_H
#define __MYOS__HARWAREDCOMMUNICATION__INTERRUPTMANAGER_H
#include <gdt.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace myos
{
    namespace hardwarecommunication
    {
        class InterruptManager;


        class InterruptHandler
        {
        protected:
            myos::common::uint8_t interrupt_number;
            InterruptManager* interrupt_manager;

            InterruptHandler(myos::common::uint8_t interrupt_number_, InterruptManager* Interrupt_manager_);
            ~InterruptHandler();

        public:
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);

        };


        class InterruptManager
        {
            friend class InterruptHandler;

        protected:

            static InterruptManager* active_interrupt_manager;
            InterruptHandler* handlers[256];
            struct GateDescriptor
            {
                myos::common::uint16_t handler_adress_low_bits;
                myos::common::uint16_t gdt_code_segment_selector;
                myos::common::uint8_t reserved;
                myos::common::uint8_t access;
                myos::common::uint16_t handler_adress_high_bits;
            } __attribute__((packed));

            static GateDescriptor interrupt_descriptor_table[256];

            struct InterruptDescriptorTablePointer
            {
                myos::common::uint16_t size;
                myos::common::uint32_t base;
            } __attribute__((packed));

            myos::common::uint16_t hardware_interrupt_offset;

            static void SetInterruptDescriptorTableEntry(
                myos::common::uint8_t interrupt_number,
                myos::common::uint16_t code_segment_selector_offset,
                void (*handler)(),
                myos::common::uint8_t descriptor_privilege_level,
                myos::common::uint8_t descriptor_type);


                Port8bitSlow pic_master_command;
                Port8bitSlow pic_master_data;
                Port8bitSlow pic_slave_command;
                Port8bitSlow pic_slave_data;


            public:
                InterruptManager(GlobalDescriptorTable* gdt);
                ~InterruptManager();

                static myos::common::uint32_t HandleInterrupt(myos::common::uint8_t interrupt_number,
                                                              myos::common::uint32_t esp);
                myos::common::uint32_t DoHandleInterrupt(myos::common::uint8_t interrupt_number,
                                                         myos::common::uint32_t esp);

                static void IgnoreInterruptRequest();
                static void HandleInterruptRequest0x01();  //keyboard int
                static void HandleInterruptRequest0x00();  //timer int
                static void HandleInterruptRequest0x0C();  //mouse int

                void Activate();
                void Deactivate();

            };
    }
}


#endif
