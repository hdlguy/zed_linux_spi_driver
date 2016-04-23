# bit2mcs.tcl
if { $::argc == 2 } {
    #write_cfgmem -force -format MCS -size 128 -interface BPIx16 -loadbit "up 0x0 [lindex $argv 1]" -verbose [lindex $argv 0]
    write_cfgmem -disablebitswap -force -format BIN -size 256 -interface SMAPx32 -loadbit "up 0x0 [lindex $argv 1]" -verbose [lindex $argv 0] 
} else {
    puts "This script converts a .bit file to a .mcs file"
    puts "USAGE: source bit2mcs.tcl <path/target.mcs> <path/source.bit>"
}


#write_cfgmem -disablebitswap -force -format BIN -size 256 -interface SMAPx32 -loadbit "up 0x0 ./results/top.bit" -verbose ./results/top.bit.bin

