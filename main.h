#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdint.h>

extern "C" void __vector_1()  __attribute__ ((signal, used, externally_visible)); //INT0
extern "C" void __vector_2()  __attribute__ ((signal, used, externally_visible)); //INT1
extern "C" void __vector_3()  __attribute__ ((signal, used, externally_visible)); //PCINT0
extern "C" void __vector_4()  __attribute__ ((signal, used, externally_visible)); //PCINT1
extern "C" void __vector_5()  __attribute__ ((signal, used, externally_visible)); //PCINT2
extern "C" void __vector_10() __attribute__ ((signal, used, externally_visible)); //TIMER1_CAPT
extern "C" void __vector_18() __attribute__ ((signal, used, externally_visible)); //USART_RX

typedef volatile uint8_t * hwAddr;

class ComPort
{
public:
    ComPort();
    void poets(const char *s);
    void putcee(char);
    void onReceive();
private:
    int addToBuffer(char);
    char *getBuffer();

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
    static const uint8_t WGM00  = 0;
    static const uint8_t WGM01  = 1;

    static const uint8_t WGM10  = 0;
    static const uint8_t WGM11  = 1;
    static const uint8_t COM1B0 = 4;
    static const uint8_t COM1B1 = 5;
    static const uint8_t COM1A0 = 6;
    static const uint8_t COM1A1 = 7;

    static const uint8_t CS10   = 0;
    static const uint8_t CS11   = 1;
    static const uint8_t CS12   = 2;
    static const uint8_t WGM12  = 3;
    static const uint8_t WGM13  = 4;

    static const uint8_t COM2A1 = 7;

    static const uint8_t CS01   = 1;
    static const uint8_t CS00   = 0;
};

class PanServo : public Servo
{
public:
    PanServo();
    void move();
    void moveTo(uint8_t);
private:
    static constexpr hwAddr const TCCR1A    = (hwAddr)0x80;
    static constexpr hwAddr const output    = (hwAddr)0x88;
    static const uint8_t ARDUINO_D9 = 1;    // PB1
};

class TiltServo : public Servo
{
public:
    TiltServo();
    void moveTo(uint8_t);
private:
    static constexpr hwAddr const output = (hwAddr)0x8a;
    static constexpr hwAddr const TCCR1B = (hwAddr)0x81;
    static const uint8_t ARDUINO_D10 = 2;

};

class PanTilt
{
public:
    PanTilt();
    void pan(int);
    void tilt(int);
private:
    PanServo panServo;
    TiltServo tiltServo;
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

class TripMeter
{
public:
    TripMeter();
    void countLeft();
    void countRight();
    unsigned int read();
    unsigned int readRight();
private:
    unsigned int left;
    unsigned int right;
    static constexpr hwAddr const portD = (hwAddr)0x2b;
    static constexpr hwAddr const eimsk = (hwAddr)0x3d;
    static constexpr hwAddr const pcicr = (hwAddr)0x68;
    static constexpr hwAddr const eicra = (hwAddr)0x69;
    static constexpr hwAddr const pcmsk0 = (hwAddr)0x6b;
    static constexpr hwAddr const pcmsk1 = (hwAddr)0x6c;
    static constexpr hwAddr const pcmsk2 = (hwAddr)0x6d;
    static const uint8_t ISC01 = 1;
    static const uint8_t INT0 = 0;
    static const uint8_t INT1 = 1;
    static const uint8_t PCINT5 = 5;
    static const uint8_t PCINT8 = 0;
    static const uint8_t PCINT9 = 1;
    static const uint8_t PCINT10 = 2;
    static const uint8_t PCINT11 = 3;
    static const uint8_t PCINT12 = 4;
    static const uint8_t PCINT13 = 5;
    static const uint8_t PCINT14 = 6;
    static const uint8_t PCINT18 = 2;
    static const uint8_t PCIE0 = 0;
    static const uint8_t PCIE1 = 1;
    static const uint8_t PCIE2 = 2;

};

class PWMPLLMotor : public Motor
{
public:
    PWMPLLMotor();
    void linksVooruit(unsigned int);
    void linksAchteruit(unsigned int);
};

class PWMMotor : public Motor
{
public:
    PWMMotor();
};

class Sonic
{
public:
    Sonic();
    unsigned int trigger();
    void sense();
    unsigned long pulseIn();
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

class Robot
{
public:
    Robot();
    void blink();
    void command(char *);
    int loop();
    ComPort *getComPort();
    TripMeter *getTripMeter();
    Sonic *getSonic();
private:
    PanTilt pt;
    ComPort comPort;
    PWMPLLMotor motor;
    static constexpr hwAddr const portB = (hwAddr)0x25;
    static constexpr hwAddr const dataDirectionB = (hwAddr)0x24;
    Sonic sonic;
    TripMeter tripMeter;
};
#endif


