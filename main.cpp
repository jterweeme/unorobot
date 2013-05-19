#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"

Robot g_robot;

ComPort::ComPort()
{
    *baudRateRegister = 103;
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
    *dataDirectionB |= (1<<ARDUINO_D9);
    *uTCCR1A = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *UNOTCCR1B = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *output = 200;
    *UNOOCR1B = 144;
}

TiltServo::TiltServo()
{
    *dataDirectionB |= (1<<ARDUINO_D11);
    *uTCCR2A = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *uTCCR2B = (1<<UWGM00) | (1<<UWGM01) | (1<<UCOM1A1) | (1<<UCS01) | (1<<UCS00);
    *uOCR2A = 244;
    *uOCR2B = 144;

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
    //motor = new PWMPLLMotor();
    comPort->poets("Lorem ipsum");
    sonic = new Sonic();
    *dataDirectionB |= (1<<5);
}

char *ComPort::getBuffer()
{
    return buffer;
}

void ComPort::onReceive()
{
    if (addToBuffer(*uUDR0) == 1)
        g_robot.command(getBuffer());
}

void Robot::blink()
{
    *portB ^= (1<<5);
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

    if (strcmp(commando, "q") == 0)
        motor.linksVooruit(deg);

    if (strcmp(commando, "a") == 0)
        motor.linksAchteruit(deg);

    if (strcmp(commando, "w") == 0)
        motor.rechtsVooruit(deg);

    if (strcmp(commando, "s") == 0)
        motor.rechtsAchteruit(deg);

    if (strcmp(commando, "d") == 0)
        sonic->trigger();
}

int Robot::loop()
{
    while (true) {
    }

    return 0;
}

PWMPLLMotor::PWMPLLMotor() : Motor()
{
}

Motor::Motor()
{
    *dataDirectionD |= (1<<4) | (1<<5) | (1<<6);
}

void Motor::linksVooruit(unsigned int speed)
{
}

void PWMPLLMotor::linksVooruit(unsigned int speed)
{
    if (speed > 20)
        *portD |= (1<<4) | (1<<5);
    else
        *portD &= ~(1<<5);
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
    g_robot.blink();
    if (speed > 20)
        *portD |= (1<<6) | (1<<7);
    else
        *portD &= ~(1<<6);
}

void Motor::rechtsAchteruit(unsigned int speed)
{
    if (speed > 20)
        *portD |= (1<<6);
    else
        *portD &= ~(1<<6);
    
    *portD &= ~(1<<7);
}

Sonic::Sonic()
{
    *dataDirectionB &= ~(1<<0);
    *dataDirectionB |= (1<<4);
    *interruptFlags = 0;
    //*timerFlags |= (1<<
}

unsigned int Sonic::trigger()
{
    *portB |= (1<<4);
    *portB &= ~(1<<4);
}

uint16_t Sonic::pulse_start;
uint16_t Sonic::pulse_width;

void Sonic::sense()
{
    if (*inputB & (1<<0))
    {
        pulse_start = *inputCapture;
        *timerFlags &= ~(1<<ICES1);
    }
    else
    {
        pulse_width = *inputCapture - pulse_start;
        *timerFlags |= (1<<ICES1);
        *counter = 0;
        char s[30];
        sprintf(s, "%d\r\n", pulse_width);
        ComPort::poets(s);
    }
}

void __vector_10()
{
    Sonic::sense();
}

void __vector_18()
{
    ComPort::onReceive();
}

int main()
{
    return g_robot.loop();
}


