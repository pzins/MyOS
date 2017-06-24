#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H
#include "gdt.h"
#include "types.h"
#include "port.h"

class InterruptManager;


class InterruptHandler
{
protected:
    uint8_t interruptNumber;
    InterruptManager* interruptManager;

    InterruptHandler(uint8_t interruptNumber, InterruptManager* InterruptManager);
    ~InterruptHandler();

public:
    virtual uint32_t HandleInterrupt(uint32_t esp);

};


class InterruptManager
{
    friend class InterruptHandler;

protected:

    static InterruptManager* ActiveInterruptManager;
    InterruptHandler* handlers[256];
    struct GateDescriptor
    {
            uint16_t handlerAdressLowBits;
            uint16_t gdt_codeSegmentSelector;
            uint8_t reserved;
            uint8_t access;
            uint16_t handlerAdressHighBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct InterruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    uint16_t hardwareInterruptOffset;

    //static InterruptManager* ActivateInterruptManager;

    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptoPrivilegeLevel,
        uint8_t DescriptorType);


        Port8bitSlow picMasterCommand;
        Port8bitSlow picMasterData;
        Port8bitSlow picSlaveCommand;
        Port8bitSlow picSlaveData;


public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();

    static uint32_t HandleInterrupt(uint8_t interruptNumber, uint32_t esp);
    uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x01();  //keyboard int
    static void HandleInterruptRequest0x00();  //timer int
    static void HandleInterruptRequest0x0C();  //timer int

    void Activate();
    void Deactivate();

};




#endif
