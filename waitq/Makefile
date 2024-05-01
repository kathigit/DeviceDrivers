obj-m += waitq_ex.o
moddir=$(shell uname -r)

all:
	@echo "moddir=${moddir}"
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
