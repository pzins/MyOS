#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"

void printf(char* str) {
    static uint16_t*  VideoMemory = (uint16_t*) 0xB8000;

    static uint8_t x = 0, y = 0;

    // *((char*)0xb8000) = 0x4c;
    // *((char*)0xb8001) = 0x07;

    // *((int*)0xb8000)=0x0759074c;
    // *((int*)0xb8004)=0x074e074f;
    // *((short*)0xb8008)=0x0720;
    // *((int*)0xb800a)=0x074c074f;

    for(int i = 0; str[i] != '\0'; ++i) {

        switch(str[i]) {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                uint16_t s = (0x0700 | str[i]);
                VideoMemory[80*y+x] = s;
                ++x;
                break;
        }
        if(x >= 80) {
            ++y;
            x = 0;
        }
        if(y >= 25) {
            for(y = 0; y < 25; ++y)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstrutors() {
     for(constructor* i = &start_ctors; i != &end_ctors; ++i)
        (*i)();

}

extern "C" void kernelMain(void* mutliboot_structure, uint32_t magicnumber)
{
    printf("Lacazette Tolisso!!!\n");
    printf("MENDY MARCAL TRAORE");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);
    KeyboardDriver keyboard(&interrupts);
    MouseDriver mouse(&interrupts);

    interrupts.Activate();


    while (1);
}
