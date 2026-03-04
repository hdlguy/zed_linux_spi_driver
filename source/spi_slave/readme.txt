This folder holds a simple spi slave to use for design verification of the Xilinx QSPI core and software drivers.

This slave uses 16 bit cycles.

first bit - 1 = write, 0 = read
next 7 bits - address, msb first.
next 8 bits - data, msb first. On read output data apears on sdo. For write data goes into sdi immediately after address.
