#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdint.h>
#include <stdlib.h>

extern "C" void __vector_10() __attribute__ ((signal, used, externally_visible)); //TIMER1_CAPT
extern "C" void __vector_18() __attribute__ ((signal, used, externally_visible)); //USART_RX

typedef volatile uint8_t * hwAddr;

inline void * operator new (size_t size)
{
    return malloc(size);
}

class ComPort
{
public:
    ComPort();
    static void poets(const char *s);
    static void putcee(char);
    static void onReceive();
private:
    static int addToBuffer(char);
    static char *getBuffer();

    static char buffer[200];
    static uint8_t buffer_ptr;
    static uint8_t last_buffer_ptr;

    static constexpr hwAddr const uUDR0             = (hwAddr)0xc6;
    static constexpr hwAddr const baudRateRegister  = (hwAddr)0xc4;
    static constexpr hwAddr const uUCSR0A           = (hwAddr)0xc0;
    static constexpr hwAddr const UUCSR0B           = (hwAddr)0xc1;

    static const uint8_t UTXEN0  = 3;
    static const uint8_t uRXEN0  = 4;
    static const uint8_t uRXCIE0 = 7;
};

class Servo
{
public:
    Servo();
protected:
    static constexpr hwAddr const dataDirectionB = (hwAddr)0x24;
    static const uint8_t UWGM00  = 0;
    static const uint8_t UWGM01  = 1;
    static const uint8_t UCOM1A1 = 7;
    static const uint8_t UCOM2A1 = 7;
    static const uint8_t UCS01   = 1;
    static const uint8_t UCS00   = 0;
};

class PanServo : public Servo
{
public:
    PanServo();
    static void move();
    static void moveTo(uint8_t);
private:
    static constexpr hwAddr const uTCCR1A   = (hwAddr)0x80;
    static constexpr hwAddr const UNOTCCR1B = (hwAddr)0x81;
    static constexpr hwAddr const output    = (hwAddr)0x88;
    static constexpr hwAddr const UNOOCR1B  = (hwAddr)0x8a;
    static const uint8_t ARDUINO_D9 = 1;    // PB1
};

class TiltServo : public Servo
{
public:
    TiltServo();
    static void moveTo(uint8_t);
private:
    static constexpr hwAddr const uTCCR2A = (volatile uint8_t *)0xb0;
    static constexpr hwAddr const uTCCR2B = (volatile uint8_t *)0xb1;
    static constexpr hwAddr const uOCR2A  = (volatile uint8_t *)0xb3;
    static constexpr hwAddr const uOCR2B  = (volatile uint8_t *)0xb4;
    static const uint8_t ARDUINO_D11 = 3;   // PB3
};

class PanTilt
{
public:
    PanTilt();
private:
    Servo *pan;
    Servo *tilt;
};

class Motor
{
public:
    Motor();
    virtual void linksVooruit(unsigned int);
    virtual void linksAchteruit(unsigned int);
    virtual void rechtsVooruit(unsigned int);
    virtual void rechtsAchteruit(unsigned int);
protected:
    static constexpr hwAddr const dataDirectionD = (hwAddr)0x2a;
    static constexpr hwAddr const portD = (hwAddr)0x2b;
};

class PWMPLLMotor : public Motor
{
public:
    PWMPLLMotor();
    void linksVooruit(unsigned int);
    void linksAchteruit(unsigned int);
};

class Sonic;

class Robot
{
public:
    Robot();
    void blink();
    void command(char *);
    int loop();
private:
    PanTilt *pt;
    ComPort *comPort;
    PWMPLLMotor motor;
    static constexpr hwAddr const portB = (hwAddr)0x25;
    static constexpr hwAddr const dataDirectionB = (hwAddr)0x24;
    Sonic *sonic;
};

class Sonic
{
public:
    Sonic();
    unsigned int trigger();
    static void sense();
private:
    static uint16_t pulse_start;
    static uint16_t pulse_width;
    static constexpr hwAddr const timerFlags     = (hwAddr)0x81;
    static constexpr hwAddr const counter        = (hwAddr)0x84;
    static constexpr hwAddr const inputCapture   = (hwAddr)0x86;
    static constexpr hwAddr const inputB         = (hwAddr)0x23;
    static constexpr hwAddr const dataDirectionB = (hwAddr)0x24;
    static constexpr hwAddr const portB          = (hwAddr)0x25;
    static constexpr hwAddr const interruptFlags = (hwAddr)0x6f;
    static const uint8_t ICES1 = 6;
};

#endif


