obj-m += main.o
obj-m += HelloWorld.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
run: all
	sudo dmesg -c
	sudo /sbin/insmod HelloWorld.ko
	sudo /sbin/insmod main.ko
	sudo /sbin/rmmod main.ko
	sudo /sbin/rmmod HelloWorld.ko
	sudo dmesg
