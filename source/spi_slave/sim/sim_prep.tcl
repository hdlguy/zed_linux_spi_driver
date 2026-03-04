# This file sets up the simulation environment.
create_project -part xc7z010clg400-1 -force sim_proj 
set_property target_language VHDL [current_project]
set_property "default_lib" "work" [current_project]

create_fileset -simset simset1
add_files -fileset simset1 [glob ../spi_slave.vhd ]
add_files -fileset simset1 [glob ../spi_slave_tb.vhd ]

read_ip [glob ../spi_slave_mem/spi_slave_mem.xci ]
generate_target {simulation} [get_ips *]
current_fileset -simset [ get_filesets simset1 ]

close_project


