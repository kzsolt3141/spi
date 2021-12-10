MCU=atmega16
F_CPU=12000000
CC=avr-gcc
OBJCOPY=avr-objcopy
MEMCHECK=avr-size
IFLAGS=-I./

CFLAGS=-std=c11 -Wall -g -O3 -mmcu=${MCU} -DF_CPU=${F_CPU} ${IFLAGS}
TARGET=main
SRCS=main.c

all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex
	${MEMCHECK} -C --mcu=${MCU} ${TARGET}.bin

clean:
	rm -f *.bin *.hex