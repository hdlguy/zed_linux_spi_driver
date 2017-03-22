# Script to compile the FPGA with zynq processor system all the way to bit file.
close_project -quiet
set outputDir ./results
file mkdir $outputDir
open_project proj.xpr

set list1 [get_ips *];
set list2 [get_ips system*];
set list3 {};
foreach core $list1 {if {[lsearch $list2 $core]==-1} { set list3 "$list3 $core" }};
puts $list3;

#synth_ip -quiet [get_ips *]

synth_design -top top
write_checkpoint -force $outputDir/post_synth.dcp

source add_ila.tcl

opt_design

place_design

phys_opt_design

route_design
write_checkpoint -force $outputDir/post_route.dcp
report_route_status -file $outputDir/post_route_status.rpt
report_timing_summary -file $outputDir/post_route_timing_summary.rpt
report_power -file $outputDir/post_route_power.rpt
report_drc -file $outputDir/post_imp_drc.rpt
report_io -file $outputDir/post_imp_io.rpt
xilinx::ultrafast::report_io_reg -verbose -file $outputDir/io_regs.rpt

#write_hwdef -force  -file ../sdk/top.hdf

write_bitstream -verbose -force $outputDir/top.bit

close_project

write_cfgmem -disablebitswap -force -format BIN -size 256 -interface SMAPx32 -loadbit "up 0x0 ./results/top.bit" -verbose ./results/top.bit.bin






