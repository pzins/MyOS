#include "port.h"



Port::Port(uint16_t port_number) {
    this->port_number = port_number;
}

Port::~Port() {

}

Port8bit::Port8bit(uint16_t port_number) : Port(port_number) {

}

Port8bit::~Port8bit() {

}

void Port8bit::Write(uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port_number));
}

uint8_t Port8bit::Read() {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd"(port_number));
    return result;
}


Port16bit::Port16bit(uint16_t port_number) : Port(port_number) {

}

Port16bit::~Port16bit() {

}

void Port16bit::Write(uint16_t data) {
    __asm__ volatile("outw %0, %1": : "a" (data), "Nd" (port_number));
}

uint16_t Port16bit::Read() {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd"(port_number));
    return result;
}


Port32bit::Port32bit(uint16_t port_number) : Port(port_number) {

}

Port32bit::~Port32bit() {

}

void Port32bit::Write(uint32_t data) {
    __asm__ volatile("outw %0, %1": : "a" (data), "Nd" (port_number));
}

uint32_t Port32bit::Read() {
    uint32_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd"(port_number));
    return result;
}


Port8bitSlow::Port8bitSlow(uint16_t port_number) : Port8bit(port_number) {

}

Port8bitSlow::~Port8bitSlow() {

}

void Port8bitSlow::Write(uint8_t data) {
    __asm__ volatile("outb %0, %1 \n jmp 1f\n1: jmp 1f\n1:": : "a" (data), "Nd" (port_number));
}
