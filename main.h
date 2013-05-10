#ifndef _MAIN_H_
#define _MAIN_H_
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
#endif


