CONFIG_MODULE_SIG=n

# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m += netdev.o

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif


#obj-m += KBUF.o

#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo /sbin/rmmod netdev.ko
	rm `pwd`/mydev

run: default
	sudo /sbin/insmod netdev.ko
	#sudo dmesg -c
	sudo mknod `pwd`/mydev c 850 50
	sudo chmod a+rwx `pwd`/mydev
	sudo ifconfig -a mynetdev 10.1.1.1/24 up
