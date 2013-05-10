#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"


ComPort::ComPort()
{
    *UUBRR0L = 103;
    *UUCSR0B = (1<<UTXEN0) | (1<<uRXEN0) | (1<<uRXCIE0);
    asm volatile ("sei" ::: "memory");
}

char ComPort::buffer[200];
uint8_t ComPort::buffer_ptr;
uint8_t ComPort::last_buffer_ptr;

void ComPort::putcee(char c)
{
    while (!(*uUCSR0A & (1<<5))) {
    }

    *uUDR0 = c;
}

void PanServo::move()
{
    (*output)--;
}

void PanServo::moveTo(uint8_t deg)
{
    *output = deg;
}

void TiltServo::moveTo(uint8_t deg)
{
    *uOCR2A = deg;
}

void ComPort::poets(const char *s)
{
    while (*s)
        putcee(*s++);
}

int ComPort::addToBuffer(char c)
{
    if (c == '\r')
    {
        buffer[buffer_ptr] = 0;
        last_buffer_ptr = buffer_ptr;
        buffer_ptr = 0;
        return 1;
    }

    buffer[buffer_ptr] = c;
    buffer_ptr++;
    return 0;
}

PanServo::PanServo()
{
    *uDDRB |= (1<<1) | (1<<2) | (1<<3) | (1<<5);
    *uTCCR1A = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *UNOTCCR1B = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *output = 244;
    *UNOOCR1B = 244;
}

TiltServo::TiltServo()
{
    *uTCCR2A = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *uTCCR2B = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *uOCR2A = 244;
    *uOCR2B = 244;

}

Servo::Servo()
{
}

PanTilt::PanTilt()
{
    pan = new PanServo();
    tilt = new TiltServo();
}

Robot::Robot()
{
    pt = new PanTilt();
    comPort = new ComPort();
    comPort->poets("Lorem ipsum");
}

char *ComPort::getBuffer()
{
    return buffer;
}

void ComPort::onReceive()
{
    if (addToBuffer(*uUDR0) == 1)
        Robot::command(getBuffer());
}

void Robot::blink()
{
    *uPORTB ^= (1<<5);
}

void Robot::command(char *cmd)
{
    char *commando = strtok((char *)cmd, " ,.-\r");
    char *parameter = strtok(NULL, " ,.-");
    unsigned int deg = atoi(parameter);
    
    if (strcmp(commando, "p") == 0)
        PanServo::moveTo(deg);

    if (strcmp(commando, "t") == 0)
        TiltServo::moveTo(deg);
}

void __vector_18()
{
    ComPort::onReceive();
}

int main()
{
    Robot robot;
    while (true) {
    }

    return 0;
}


