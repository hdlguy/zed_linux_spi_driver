#!/bin/bash
##reading temperature sensor
##in kernel configure, setting CONFIG_XILINX_XADC=y with make xconfig
##the srouce codes are xilinx-xadc-core.c xilinx-xadc-events.c xilinx-xadc.h in /linux-xlnx/drivers/iio/adc

while [ 1 ]; do
    echo read in_temp0_scale: `cat /sys/devices/amba.0/f8007100.ps7-xadc/iio:device0/in_temp0_scale`
    echo read in_temp0_offset: `cat /sys/devices/amba.0/f8007100.ps7-xadc/iio:device0/in_temp0_offset`
    RAW_VALUE=`cat /sys/devices/amba.0/f8007100.ps7-xadc/iio:device0/in_temp0_raw`
    TEMP_C=$(echo "$RAW_VALUE*503.975/4096-273.15" | bc )
    echo read in_temp0_raw: $RAW_VALUE, TEMP. IN C: $TEMP_C 
    echo
    sleep 5
done
