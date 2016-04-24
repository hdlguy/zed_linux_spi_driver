This folder contains to the files to make a boot.bin file that contains the FSBL and u-boot images. 

First do something like this to setup the path to the Xilinx SDK executables.

source /opt/Xilinx/SDK/2016.1/settings64.sh

Then run this command to build boot.bin and create a new devicetree.dtb (device tree blob).

./genscript


