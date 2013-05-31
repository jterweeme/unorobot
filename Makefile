all: main.hex

main.hex: main
	avr-objcopy -O ihex -R .eeprom $< $@

main: main.o motor.o
	avr-g++ -mmcu=atmega328p -o $@ $^

main.o: main.cpp main.h
	avr-g++ -c -O2 -std=c++11 -mmcu=atmega328p -o $@ $<

motor.o: motor.cpp main.h
	avr-g++ -c -O2 -std=c++11 -mmcu=atmega328p -o $@ $<

upload: main.hex
	avrdude -c arduino -p m328p -P /dev/ttyACM0 -U $<

clean:
	rm -v main.o main



