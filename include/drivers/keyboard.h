#ifndef __MYOS_DRIVERS__KEYBOARD_H
#define __MYOS_DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace myos
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
        public:
            // KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };



        class KeyboardDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
        {
            myos::hardwarecommunication::Port8bit data_port;
            myos::hardwarecommunication::Port8bit command_port;

            KeyboardEventHandler* handler;

        public:
            KeyboardDriver(myos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler_);
            ~KeyboardDriver();
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}



#endif
