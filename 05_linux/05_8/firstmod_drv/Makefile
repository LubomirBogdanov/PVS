MODNAME_DRV = 
CPU	= arm
CCPWD	= /path/to/STM32F769I-disco_Buildroot/buildroot/output/host/bin/arm-buildroot-uclinux-uclibcgnueabi-
KERNDIR = /path/to/STM32F769I-disco_Buildroot/buildroot/output/build/linux-custom

obj-m += ${MODNAME_DRV}.o

all:
	mkdir -p ./debug
	make ARCH=${CPU} LOCALVERSION=${LINVERSION} CROSS_COMPILE=${CCPWD} -C ${KERNDIR} SUBDIRS=$(PWD) modules
	mv ${MODNAME_DRV}.mod.c ${MODNAME_DRV}.ko ${MODNAME_DRV}.mod.o ${MODNAME_DRV}.o Module.symvers modules.order ./debug
clean:
	rm -r ./debug
