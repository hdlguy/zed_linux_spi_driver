# Script to compile the FPGA with zynq processor system all the way to bit file.

close_project -quiet

open_project proj.xpr

update_compile_order -fileset sources_1
#launch_runs impl_1 -to_step write_bitstream -jobs 8

reset_run synth_1
launch_runs synth_1 -jobs 8
wait_on_run synth_1

launch_runs impl_1 -jobs 8
wait_on_run impl_1

open_run impl_1
report_timing_summary   -file   ./results/timing.rpt
report_utilization      -file   ./results/utilization.rpt
write_debug_probes      -force  ./results/top.ltx
write_mem_info          -force  ./results/top.mmi



write_bitstream -verbose -force -file ./results/top.bit
file copy ./results/top.bit ./proj.runs/impl_1/

write_hw_platform -fixed -include_bit -force -file ./results/top.xsa
#write_hw_platform -fixed -force -file ./results/top.xsa

close_project



