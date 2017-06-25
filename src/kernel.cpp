#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

void printf(char* str) {
    static uint16_t*  video_memory = (uint16_t*) 0xB8000;

    static uint8_t x = 0, y = 0;
    //define color : foreground 4 LSB and background 3 MSB
    uint16_t color = 0x10;

    //loop over the string
    for(int i = 0; str[i] != '\0'; ++i) {
        switch(str[i]) {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                uint16_t s = ((color << 8) | str[i]);
                video_memory[80 * y + x] = s;
                ++x;
                break;
        }
        // jump to new line
        if(x >= 80) {
            ++y;
            x = 0;
        }
        // screen full : clean and goto begin
        if(y >= 25) {
            for(y = 0; y < 25; ++y)
                for(x = 0; x < 80; x++)
                    video_memory[80*y+x] = (video_memory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printHex(uint8_t n) {
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(n >> 4) & 0xF];
    foo[1] = hex[n & 0xF];
    printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c) {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }

    void OnKeyUp(char c) {

    }
};

class MouseToConsole : public MouseEventHandler
{
private:
    int8_t x, y;
public:

    MouseToConsole()
     {
         uint16_t* video_memory = (uint16_t*)0xb8000;
         x = 40;
         y = 12;
         video_memory[80*y+x] = (~video_memory[80*y+x] & 0x0F00) << 4
                              | (video_memory[80*y+x] & 0xF000) >> 4
                              | (video_memory[80*y+x] & 0x00FF);
     }

     virtual void OnMouseMove(int x_offset, int y_offset)
     {
         static uint16_t* video_memory = (uint16_t*)0xb8000;
         video_memory[80*y+x] = (video_memory[80*y+x] & 0x0F00) << 4
                              | (video_memory[80*y+x] & 0xF000) >> 4
                              | (video_memory[80*y+x] & 0x00FF);

         x += x_offset;
         if(x >= 80) x = 79;
         if(x < 0) x = 0;
         y += y_offset;
         if(y >= 25) y = 24;
         if(y < 0) y = 0;

         video_memory[80*y+x] = (video_memory[80*y+x] & 0x0F00) << 4
                              | (video_memory[80*y+x] & 0xF000) >> 4
                              | (video_memory[80*y+x] & 0x00FF);
     }
    void OnMouseDown(uint8_t button) {

    }
    void OnMouseUp(uint8_t button) {

    }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstrutors() {
     for(constructor* i = &start_ctors; i != &end_ctors; ++i)
        (*i)();
}

extern "C" void kernelMain(void* mutliboot_structure, uint32_t magicnumber)
{
    printf("Lacazette");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);
    printf("Initializing Hardware, Stage 1\n");

    DriverManager drv_manager;

    PrintfKeyboardEventHandler kb_handler;
    KeyboardDriver keyboard(&interrupts, &kb_handler);
    drv_manager.AddDriver(&keyboard);

    MouseToConsole mouse_handler;
    MouseDriver mouse(&interrupts, &mouse_handler);
    drv_manager.AddDriver(&mouse);
    printf("Initializing Hardware, Stage 2\n");

    drv_manager.ActivateAll();
    interrupts.Activate();
    printf("Initializing Hardware, Stage 3\n");


    while (1);
}
