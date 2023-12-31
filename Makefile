CC=gcc -m64
obj-m+= basic_driver.o

KDIR=/lib/modules/6.1.0-rpi7-rpi-v7/build/

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -c $(KDIR) M=$(shell pwd) clean


