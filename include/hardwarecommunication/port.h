#ifndef __MYOS__HARWAREDCOMMUNICATION__PORT_H
#define __MYOS__HARWAREDCOMMUNICATION__PORT_H

#include <common/types.h>

namespace myos
{
    namespace hardwarecommunication
    {
        class Port
        {
        protected:
            myos::common::uint16_t port_number;
            Port(myos::common::uint16_t port_number);
            ~Port();
        };

        class Port8bit : public Port
        {
        public:
            Port8bit(myos::common::uint16_t port_number);
            ~Port8bit();
            virtual void Write(myos::common::uint8_t data);
            virtual myos::common::uint8_t Read();
        };

        class Port8bitSlow : public Port8bit
        {
        public:
            Port8bitSlow(myos::common::uint16_t port_number);
            ~Port8bitSlow();
            virtual void Write(myos::common::uint8_t data);
        };

        class Port16bit : public Port
        {
        public:
            Port16bit(myos::common::uint16_t port_number);
            ~Port16bit();
            virtual void Write(myos::common::uint16_t data);
            virtual myos::common::uint16_t Read();
        };

        class Port32bit : public Port
        {
        public:
            Port32bit(myos::common::uint16_t port_number);
            ~Port32bit();
            virtual void Write(myos::common::uint32_t data);
            virtual myos::common::uint32_t Read();
        };
    }
}




#endif
