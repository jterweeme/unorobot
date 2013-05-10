#include <stdlib.h>
#include <stdint.h>

extern "C" void __vector_18() __attribute__ ((signal, used, externally_visible));

void * operator new (size_t size)
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
};

class PanServo : public Servo
{
public:
    PanServo();
    static void move();
private:
    static constexpr volatile uint8_t * const UNODDRB   = (volatile uint8_t *)0x24;
    static constexpr volatile uint8_t * const UNOTCCR1A = (volatile uint8_t *)0x80;
    static constexpr volatile uint8_t * const UNOTCCR1B = (volatile uint8_t *)0x81;
    static constexpr volatile uint8_t * const UNOTCCR2A = (volatile uint8_t *)0xb0;
    static constexpr volatile uint8_t * const UNOTCCR2B = (volatile uint8_t *)0xb1;
    static constexpr volatile uint8_t * const UNOOCR1A  = (volatile uint8_t *)0x88;
    static constexpr volatile uint8_t * const UNOOCR1B  = (volatile uint8_t *)0x8a;
    static constexpr volatile uint8_t * const UNOOCR2A  = (volatile uint8_t *)0xb3;

    static const uint8_t UWGM00  = 0;
    static const uint8_t UWGM01  = 1;
    static const uint8_t UCOM1A1 = 7;
    static const uint8_t UCOM2A1 = 7;
    static const uint8_t UCS01   = 1;
    static const uint8_t UCS00   = 0;
};

class PanTilt
{
public:
    PanTilt();
private:
    Servo *pan;
    Servo *tilt;
};

class Robot
{
public:
    Robot();
private:
    PanTilt *pt;
    ComPort *comPort;
    volatile uint8_t *uPORTB = (volatile uint8_t *)0x25;

};

ComPort::ComPort()
{
    *UUBRR0L = 103;
    *UUCSR0B = (1<<UTXEN0) | (1<<uRXEN0) | (1<<uRXCIE0);
    asm volatile ("sei" ::: "memory");
}

void ComPort::putcee(char c)
{
    while (!(*uUCSR0A & (1<<5))) {
    }

    *uUDR0 = c;
}

void PanServo::move()
{
    (*UNOOCR1A)--;
}

void ComPort::poets(const char *s)
{
    while (*s)
    {
        putcee(*s++);
    }
}

int ComPort::addToBuffer(char c)
{
    return 0;
}

PanServo::PanServo()
{
    *UNODDRB |= (1<<1) | (1<<2) | (1<<3) | (1<<5);
    *UNOTCCR1A = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *UNOTCCR1B = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *UNOTCCR2A = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM2A1) | (1<<UCS01) | (1<<UCS00);
    *UNOTCCR2B = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM2A1) | (1<<UCS01) | (1<<UCS00);
    *UNOOCR1A = 244;
    *UNOOCR1B = 44;
    *UNOOCR2A = 44;
}

Servo::Servo()
{
}

PanTilt::PanTilt()
{
    pan = new PanServo();
    tilt = new Servo();
}

Robot::Robot()
{
    pt = new PanTilt();
    comPort = new ComPort();
    comPort->poets("Lorem ipsum");
}

void ComPort::onReceive()
{
    uint8_t onzin = *uUDR0;
    PanServo::move();
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


