#include "main.h"
void PWMMotor::rechtsAchteruit(unsigned int speed)
{
    *portD &= ~(1<<7);
    *OCR0A = speed;
}

Motor::Motor()
{
    *dataDirectionD |= (1<<4) | (1<<5) | (1<<6);
}

void Motor::linksVooruit(unsigned int speed)
{
}

void PWMPLLMotor::linksAchteruit(unsigned int speed)
{
    *portD &= ~(1<<4);

    if (speed > 20)
        *portD |= (1<<5);
    else
        *portD &= ~(1<<5);
}

void Motor::linksAchteruit(unsigned int speed)
{
}

void Motor::rechtsVooruit(unsigned int speed)
{
    if (speed > 20)
        *portD |= (1<<6) | (1<<7);
    else
        *portD &= ~(1<<6);
}

void PWMPLLMotor::linksVooruit(unsigned int speed)
{
    if (speed > 20)
        *portD |= (1<<4) | (1<<5);
    else
        *portD &= ~(1<<5);
}

PWMPLLMotor::PWMPLLMotor() : Motor()
{
}

PWMMotor::PWMMotor() : Motor()
{
    *TCCR0A = (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
    *TCCR0B = (1<<WGM01) | (1<<CS00);
}

void PWMMotor::linksVooruit(unsigned int speed)
{
    *portD |= (1<<4);
    *OCR0B = speed;
}

void PWMMotor::linksAchteruit(unsigned int speed)
{
    *portD &= ~(1<<4);
    *OCR0B = speed;
}

void PWMMotor::rechtsVooruit(unsigned int speed)
{
    *portD |= (1<<7);
    *OCR0A = speed;
}

void Motor::rechtsAchteruit(unsigned int speed)
{
    if (speed > 20)
        *portD |= (1<<6);
    else
        *portD &= ~(1<<6);

    *portD &= ~(1<<7);
}



