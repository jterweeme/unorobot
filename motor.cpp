#include "main.h"
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


