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
{   (*output)--;
}

void PanServo::moveTo(uint8_t deg)
{   *output = deg;
}

void TiltServo::moveTo(uint8_t deg)
{   *uOCR2A = deg;
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
        sonic.trigger();

    if (strcmp(commando, "o") == 0)
    {
        char s[30];
        sprintf(s, "%d - %d\r\n", tripMeter.read(), tripMeter.readRight());
        comPort.poets(s);
    }
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

TripMeter::TripMeter()
{
    left = 0;
    right = 0;
    *portD |= (1<<2) | (1<<3);
    *eicra |= (1<<ISC01);
    *eimsk |= (1<<INT0) | (1<<INT1);    // enable external interupt 0 & 1
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
    //*timerFlags |= (1<<
}

unsigned int Sonic::trigger()
{
    *portB &= ~(1<<4);
    *portB |= (1<<4);
    unsigned long distanceMeasured = pulseIn();
    return 0;
}

uint16_t Sonic::pulse_start;
uint16_t Sonic::pulse_width;

void Sonic::sense()
{
    
}

void __vector_1()
{   g_robot.getTripMeter()->countLeft();
}


void __vector_2()
{   g_robot.getTripMeter()->countRight();
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


