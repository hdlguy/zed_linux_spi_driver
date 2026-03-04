# This makefile demonstrates the use of Gnu Make to compile FPGA designs using Xilinx Vivado.
# Watch out! Gnu Make uses the tab character to indicate commands to execute. If your editor replaces 
# the tabs with spaces the makefile will not run.
all: ./results/top.bin

cores: ../source/spi_slave/spi_slave_mem/spi_slave_mem.dcp

./results/top.bit: ../source/top.v ../source/top.xdc  ./results/system.dcp ../source/spi_slave/spi_slave.vhd ../source/spi_slave/spi_slave_mem/spi_slave_mem.dcp
	vivado -nolog -nojournal -mode batch -notrace -source vcomp.tcl -tclargs $@ $^

./results/system.dcp: ../source/system.tcl
	vivado -nolog -nojournal -mode batch -notrace -source bd2dcp.tcl -tclargs $@ $<

#./results/sig_gen.dcp: ../source/sig_gen.v ../cores/dds_compiler_0/dds_compiler_0.dcp ../cores/c_counter_binary_0/c_counter_binary_0.dcp ../source/sig_gen.xdc
#	vivado -nolog -nojournal -mode batch -notrace -source vcomp.tcl -tclargs $@ $^

%.bin: %.bit
	vivado -nolog -nojournal -mode batch -notrace -source bit2bin.tcl -tclargs $@ $<

#%.mcs: %.bit
#	vivado -nolog -nojournal -mode batch -notrace -source bit2mcs.tcl -tclargs $@ $<

%.dcp: %.xci
	vivado -nolog -nojournal -mode batch -notrace -source xci2dcp.tcl -tclargs $<

.PHONY: clean
clean:
	rm --force \
	./results/system.dcp \
	../source/spi_slave/spi_slave_mem/spi_slave_mem.dcp \
	./results/top.mcs ./results/top.bit 


