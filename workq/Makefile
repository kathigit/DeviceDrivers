obj-m += workq_with_irq.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make /lib/modules/$(uname -r)/build M=$(PWD) clean
