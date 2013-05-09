#include <stdlib.h>
#include <avr/io.h>
void * operator new (size_t size)
{
    return malloc(size);
}

class Servo
{
public:
    Servo();
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
};

Servo::Servo()
{
    TCCR2B = (1<<WGM01) | (1<<COM2B0) | (1<<CS01);
    OCR2B = 24;
    DDRD = (1<<3);
}

PanTilt::PanTilt()
{
    pan = new Servo();
}

Robot::Robot()
{
    pt = new PanTilt();
}

int main()
{
    Robot robot;
    return 0;
}


