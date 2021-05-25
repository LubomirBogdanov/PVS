#Created on 20 Oct 2020 by Lubomir Bogdanov
#This is an example and simple Makefile

all: main.bin list size

.PHONY: menuconfig

menuconfig:
	./menuconfig/build/mconf Kconfig
	./menuconfig/build/conf Kconfig --silentoldconfig autoconf.h

main.bin: main.axf
	arm-none-eabi-objcopy -O binary ./debug/main.axf ./debug/main.bin

main.axf: debug main.o startup.o led.o uart.o printf.o
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -nostartfiles --specs=nosys.specs -mfloat-abi=soft -Tscript.ld ./debug/main.o ./debug/led.o ./debug/uart.o ./debug/startup.o ./debug/printf.o -o ./debug/main.axf

debug:
	mkdir -p debug

main.o: main.s
	arm-none-eabi-as -mcpu=cortex-m7 -mthumb -I./uart -I./led -I./printf ./debug/main.s -o ./debug/main.o

main.s: 
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -nostartfiles --specs=nosys.specs -mfloat-abi=soft -O0 -g -DPRINTF_MESSAGE=3 -S -I./uart -I./led -I./printf main.c -o ./debug/main.s
	
startup.o:
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -nostartfiles --specs=nosys.specs -mfloat-abi=soft -c startup.c -o ./debug/startup.o
	
led.o:
	${MAKE} -C led
	
uart.o:
	${MAKE} -C uart

printf.o:
	${MAKE} -C printf
		
flash: main.bin
	openocd -f board/stm32f7discovery.cfg -c"init" -c"reset halt" -c"flash probe 0" -c"flash write_image erase ./debug/main.bin 0x08000000" -c"reset run" -c"exit"

clean:
	rm -r debug 
	
list: main.o
	arm-none-eabi-objdump -S -D ./debug/main.o > ./debug/main.lst
	
size: main.axf	
	arm-none-eabi-size ./debug/main.axf	
