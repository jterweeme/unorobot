all: main.hex

main.hex: main
	avr-objcopy -O ihex -R .eeprom $< $@

main: main.o
	avr-g++ -mmcu=atmega328p -o $@ $^

main.o: main.cpp
	avr-g++ -c -O2 -mmcu=atmega328p -o $@ $<

upload: main.hex
	avrdude -c arduino -p m328p -P /dev/ttyACM0 -U $<

unorobot.svg: unorobot.uxf
	umlet -action=convert -format=svg -filename=$<

documentation.pdf: documentatie.xml pdf.xsl unorobot.svg
	fop -xsl pdf.xsl -xml documentatie.xml -pdf $@



