#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"


class MouseEventHandler
{
public:
    MouseEventHandler();
    virtual void OnActivate();
    virtual void OnMouseDown(uint8_t button);
    virtual void OnMouseUp(uint8_t button);
    virtual void OnMouseMove(int offset_x, int offset_y);
};

class MouseDriver : public InterruptHandler, public Driver
{
    Port8bit data_port;
    Port8bit command_port;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    MouseEventHandler* handler;

public:
    MouseDriver(InterruptManager* manager, MouseEventHandler* handler_);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();

};



#endif
