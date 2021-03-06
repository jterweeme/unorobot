#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

Robot g_robot;

ComPort::ComPort()
{
    *baudRateRegister = 103;
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
{   (*output)--;
}

void PanServo::moveTo(uint8_t deg)
{   *output = deg;
}

void TiltServo::moveTo(uint8_t deg)
{   *output = deg;
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
    *dataDirectionD |= (1<<ARDUINO_D3);
    *TCCR2A = (1<<COM2A1) | (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);
    *TCCR2B = (1<<CS21) | (1<<CS22);
    *output = 200;
}

TiltServo::TiltServo()
{
    *dataDirectionB |= (1<<ARDUINO_D11);
    *output = 100;
}

Servo::Servo()
{
}

PanTilt::PanTilt()
{
}

void PanTilt::pan(int deg)
{
    panServo.moveTo(deg);
}

void PanTilt::tilt(int deg)
{
    tiltServo.moveTo(deg);
}

Robot::Robot()
{
    comPort.poets("StartUp...\r\n");
    *dataDirectionB |= (1<<5);
}

char *ComPort::getBuffer()
{   return buffer;
}

void ComPort::onReceive()
{
    if (addToBuffer(*uUDR0) == 1)
        g_robot.command(getBuffer());
}

void Robot::blink()
{   *portB ^= (1<<5);
}

ComPort *Robot::getComPort()
{   return &comPort;
}

Sonic *Robot::getSonic()
{   return &sonic;
}

uint16_t Sonic::bogus(uint8_t port)
{
    uint16_t result;
    return 0;
}


unsigned long Sonic::pulseIn()
{
    return 0;
}

void Robot::command(char *cmd)
{
    char *commando = strtok((char *)cmd, " ,.-\r");
    char *parameter = strtok(NULL, " ,.-");
    unsigned int deg = atoi(parameter);
    
    if (strcmp(commando, "p") == 0)
        pt.pan(deg);

    if (strcmp(commando, "t") == 0)
        pt.tilt(deg);

    if (strcmp(commando, "q") == 0)
        motor.linksVooruit(deg);

    if (strcmp(commando, "a") == 0)
        motor.linksAchteruit(deg);

    if (strcmp(commando, "w") == 0)
        motor.rechtsVooruit(deg);

    if (strcmp(commando, "s") == 0)
        motor.rechtsAchteruit(deg);

    if (strcmp(commando, "d") == 0)
    {
        unsigned long duur = sonic.sense();
        char s[50];
        sprintf(s, "s%lus", duur);
        comPort.poets(s);
    }

    if (strcmp(commando, "o") == 0)
    {
        char s[30];
        sprintf(s, "t%d - %dt", tripMeter.read(), tripMeter.readRight());
        comPort.poets(s);
    }
    
    if (strcmp(commando, "x") == 0)
        comPort.poets("Cookie");
}

TripMeter *Robot::getTripMeter()
{   return &tripMeter;
}

int Robot::loop()
{
    while (true) {
    }

    return 0;
}

TripMeter::TripMeter()
{
    left = 0;
    right = 0;
    *pcicr |= (1<<PCIE1) | (1<<PCIE2);
    *pcmsk1 |= (1<<PCINT13);
    *pcmsk2 |= (1<<PCINT18);
}

void TripMeter::countLeft()
{   left++;
}

void TripMeter::countRight()
{   right++;
}

unsigned int TripMeter::read()
{   return left;
}

unsigned int TripMeter::readRight()
{   return right;
}

Sonic::Sonic()
{
    *dataDirectionB &= ~(1<<0);
    *dataDirectionB |= (1<<4);
    *interruptFlags = 0;
}

unsigned int Sonic::trigger()
{
    *portB &= ~(1<<4);
    *portB |= (1<<4);
    unsigned long distanceMeasured = pulseIn();
    return 0;
}

unsigned long Sonic::sense()
{
    return 123456789;
}

void __vector_3()
{   g_robot.getTripMeter()->countLeft();
}

void __vector_4()
{   g_robot.getTripMeter()->countRight();
}

void __vector_5()
{   g_robot.getTripMeter()->countLeft();
}

void __vector_10()
{   g_robot.getSonic()->sense();
}


void __vector_18()
{   g_robot.getComPort()->onReceive();
}


int main() {
    return g_robot.loop();
}


