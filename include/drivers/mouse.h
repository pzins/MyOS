#ifndef __MYOS_DRIVERS__MOUSE_H
#define __MYOS_DRIVERS__MOUSE_H


#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace myos
{
    namespace drivers
    {
        class MouseEventHandler
        {
        public:
            MouseEventHandler();
            virtual void OnActivate();
            virtual void OnMouseDown(myos::common::uint8_t button);
            virtual void OnMouseUp(myos::common::uint8_t button);
            virtual void OnMouseMove(int offset_x, int offset_y);
        };

        class MouseDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
        {
            myos::hardwarecommunication::Port8bit data_port;
            myos::hardwarecommunication::Port8bit command_port;

            myos::common::uint8_t buffer[3];
            myos::common::uint8_t offset;
            myos::common::uint8_t buttons;

            MouseEventHandler* handler;

        public:
            MouseDriver(myos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler_);
            ~MouseDriver();
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
            virtual void Activate();

        };
    }
}



#endif
