MODNAME = 
CPU	= arm
CCPWD	= 
KERNDIR = 

obj-m += ${MODNAME}.o

all:
	mkdir -p ./debug
	make ARCH=${CPU} LOCALVERSION=${LINVERSION} CROSS_COMPILE=${CCPWD} -C ${KERNDIR} SUBDIRS=$(PWD) modules
	mv ${MODNAME}.mod.c ${MODNAME}.ko ${MODNAME}.mod.o ${MODNAME}.o Module.symvers modules.order ./debug
clean:
	rm -r ./debug
