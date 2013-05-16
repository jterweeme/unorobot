#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdint.h>
#include <stdlib.h>

extern "C" void __vector_18() __attribute__ ((signal, used, externally_visible));

inline void * operator new (size_t size)
{
    return malloc(size);
}

class ComPort
{
public:
    ComPort();
    void poets(const char *s);
    void putcee(char);
    static void onReceive();
private:
    static int addToBuffer(char);
    static char *getBuffer();

    static char buffer[200];
    static uint8_t buffer_ptr;
    static uint8_t last_buffer_ptr;

    static constexpr volatile uint8_t * const uUDR0   = (volatile uint8_t *)0xc6;
    static constexpr volatile uint8_t * const UUBRR0L = (volatile uint8_t *)0xc4;
    static constexpr volatile uint8_t * const uUCSR0A = (volatile uint8_t *)0xc0;
    static constexpr volatile uint8_t * const UUCSR0B = (volatile uint8_t *)0xc1;

    static const uint8_t UTXEN0  = 3;
    static const uint8_t uRXEN0  = 4;
    static const uint8_t uRXCIE0 = 7;
};

class Servo
{
public:
    Servo();
protected:
    static constexpr volatile uint8_t * const uDDRB   = (volatile uint8_t *)0x24;
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
    static constexpr volatile uint8_t * const uTCCR1A   = (volatile uint8_t *)0x80;
    static constexpr volatile uint8_t * const UNOTCCR1B = (volatile uint8_t *)0x81;
    static constexpr volatile uint8_t * const output = (volatile uint8_t *)0x88;
    static constexpr volatile uint8_t * const UNOOCR1B  = (volatile uint8_t *)0x8a;

};

class TiltServo : public Servo
{
public:
    TiltServo();
    static void moveTo(uint8_t);
private:
    static constexpr volatile uint8_t * const uTCCR2A = (volatile uint8_t *)0xb0;
    static constexpr volatile uint8_t * const uTCCR2B = (volatile uint8_t *)0xb1;
    static constexpr volatile uint8_t * const uOCR2A  = (volatile uint8_t *)0xb3;
    static constexpr volatile uint8_t * const uOCR2B  = (volatile uint8_t *)0xb4;

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
    static constexpr volatile uint8_t * const uDDRD = (volatile uint8_t *)0x2a;
    static constexpr volatile uint8_t * const uPORTD = (volatile uint8_t *)0x2b;
};

class PWMPLLMotor : public Motor
{
public:
    PWMPLLMotor();
    void linksVooruit(unsigned int);
    void linksAchteruit(unsigned int);
};

class Robot
{
public:
    Robot();
    static void blink();
    void command(char *);
private:
    PanTilt *pt;
    ComPort *comPort;
    Motor *motor;
    static constexpr volatile uint8_t * const uPORTB = (volatile uint8_t *)0x25;

};

#endif


