#include <drivers/mouse.h>
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

MouseEventHandler::MouseEventHandler() {

}

void MouseEventHandler::OnActivate() {

}

void MouseEventHandler::OnMouseDown(uint8_t button) {

}

void MouseEventHandler::OnMouseUp(uint8_t button) {

}

void MouseEventHandler::OnMouseMove(int offset_x, int offset_y) {

}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler_) :
InterruptHandler(0x2C, manager),
handler(handler_),
data_port(0x60),
command_port(0x64)
{

}



MouseDriver::~MouseDriver() {

}

void MouseDriver::Activate() {
    offset = 0;
    buttons = 0;

    if(handler != 0)
        handler->OnActivate();

    command_port.Write(0xA8); //activate mouse interrupt
    command_port.Write(0x20); //get current state
    uint8_t status = data_port.Read() | 2; //set second bit to true
    command_port.Write(0x60); //set state
    data_port.Write(status);

    command_port.Write(0xD4);
    data_port.Write(0xF4);
    data_port.Read();
}

void printf(char*);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = command_port.Read();
    //test if there are data to read
    if(!(status & 0x20))
        return esp;

    buffer[offset] = data_port.Read();

    if(handler == 0)
        return esp;

    offset = (offset + 1) % 3;

    if(offset == 0) {

        if(buffer[1] != 0 || buffer[2] != 0)
            handler->OnMouseMove(buffer[1], -buffer[2]);

        for(uint8_t i = 0; i < 3; ++i)
        {
            if((buffer[0] & (0x1 << i)) != (buttons & (0x1 < i))) {
                if(buttons & (0x1 << 1))
                    handler->OnMouseUp(i+1);
                else
                    handler->OnMouseDown(i+1);
            }
        }
        buttons = buffer[0];
    }
    return esp;
}
