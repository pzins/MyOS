#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

class KeyboardEventHandler
{
public:
    KeyboardEventHandler();
    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);

};



class KeyboardDriver : public InterruptHandler, public Driver
{
    Port8bit data_port;
    Port8bit command_port;

    KeyboardEventHandler* handler;

public:
    KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler_);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};



#endif
