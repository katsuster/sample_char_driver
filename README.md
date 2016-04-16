# sample_char_driver

This is a sample code for Linux character device driver.
A driver allocates major number dynamically.

# How to Use

Get the Linux kernel source and extract to /usr/src/linux.

* ex) apt-get install linux-source
* ex) wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-x.y.z.tar.xz

Build Linux kernel.

    # cd /usr/src/linux
    # make

Build this sample.

    $ cd sample_char_driver
    $ make
    make ARCH=x86_64 CROSS_COMPILE= -C /usr/src/linux M=/home/user/sample_platform_bus_driver LDDINC=/home/user/sample_char_driver modules
    make[1]: Entering directory '/usr/src/linux'
      CC [M]  /home/user/sample_char_driver/char-device.o
      CC [M]  /home/user/sample_char_driver/char-utils.o
      ...
      CC      /home/user/sample_char_driver/sample-char-dev.mod.o
      LD [M]  /home/user/sample_char_driver/sample-char-dev.ko
    make[1]: Leaving directory '/usr/src/linux'

Do insmod and rmmod.

    # insmod sample-char-dev.ko
    sample_char: loaded.
    
    # rmmod sample_char_dev
    sample_char: unloaded.
