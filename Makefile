all: main.hex unorobot.svg unorobot.png

main.hex: main
	avr-objcopy -O ihex -R .eeprom $< $@

main: main.o
	avr-g++ -mmcu=atmega328p -o $@ $^

main.o: main.cpp main.h
	avr-g++ -c -O2 -std=c++11 -mmcu=atmega328p -o $@ $<

upload: main.hex
	avrdude -c arduino -p m328p -P /dev/ttyACM0 -U $<

unorobot.svg: unorobot.uxf
	umlet -action=convert -format=svg -filename=$<

unorobot.png: unorobot.uxf
	umlet -action=convert -format=png -filename=$<

clean:
	rm -v documentation.pdf main.o main



