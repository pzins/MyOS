#include "mouse.h"


MouseEventHandler::MouseEventHandler() {

}

void MouseEventHandler::onActivate() {

}

void MouseEventHandler::onMouseDown(uint8_t button) {

}

void MouseEventHandler::onMouseUp(uint8_t button) {

}

void MouseEventHandler::onMouseMove(int offsetx, int offsety) {

}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler_)
: InterruptHandler(0x2C, manager),
handler(handler_),
dataport(0x60),
commandport(0x64)
{

}



MouseDriver::~MouseDriver() {
}

void MouseDriver::Activate() {


    commandport.Write(0xA8); //activate mouse interrupt
    commandport.Write(0x20); //get current state
    uint8_t status = dataport.Read() | 2; //set second bit to true
    commandport.Write(0x60); //set state
    dataport.Write(status);
    commandport.Write(0xD4);
    dataport.Write(0xf4);
    dataport.Read();
}

void printf(char*);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = commandport.Read();
    //test if there are data to read
    if(!(status & 0x20))
        return esp;

    static int8_t x=40, y=12;

    buffer[offset] = dataport.Read();
    if(handler == 0)
        return esp;
    offset = (++offset) % 3;

    if(offset == 0) {

        handler->onMouseMove(buffer[1], -buffer[2]);

        for(uint8_t i = 0; i < 3; ++i)
        {
            if((buffer[0]& (0x01 << i)) != (buttons & (0x01 < i))){
                if(buttons & (0x1<<1))
                    handler->onMouseUp(i+1);
                else
                    handler->onMouseDown(i+1);
            }
        }
/*
        for(uint8_t i = 0; i < 3; ++i)
        {
            if((buffer[0]& (0x01 << i)) != (buttons & (0x01 < i))){
                VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) |
                ((VideoMemory[80*y+x] & 0x0F00) << 4) |
                (VideoMemory[80*y+x] & 0x00FF);
            }
        }
*/
        buttons = buffer[0];
    }
    return esp;
}
