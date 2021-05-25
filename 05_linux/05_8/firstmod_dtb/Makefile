DTC_PATH = /path/to/STM32F769I-disco_Buildroot/buildroot/output/build/linux-custom/scripts/dtc

all: firstmod.dtb

.PHONY: firstmod.dtb

firstmod.dtb: firstmod.dts
	cpp -I./ -E -P -x assembler-with-cpp firstmod.dts | ${DTC_PATH}/dtc -I dts -O dtb -o firstmod.dtb -

preprocess_only:
	cpp -I./ -E -P -x assembler-with-cpp firstmod.dts
