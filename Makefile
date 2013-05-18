all: main.hex documentation.pdf

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

documentation.pdf: documentation.xml pdf.xsl unorobot.svg
	fop -xsl pdf.xsl -xml documentation.xml -pdf $@

clean:
	rm -v documentation.pdf main.o main



