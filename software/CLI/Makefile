ALL_EXE = cli cli_w cli_r mem_io config_obj tx_ram_load tx_ram_unload
all: $(ALL_EXE) 

CC=gcc
CFLAGS=-I. -I../API
DEPS = ../API/rf_proto_hw.h ../API/phy_2_vir_addr.h ../API/utils.h ../API/ram_mgr.h mem_access.h optstring.h Makefile

PUBLIC_LIBS = ../API/phy_2_vir_addr.o ../API/utils.o
LIB_FILES := mem_access.c
LIB_OBJS := $(subst .c,.o,$(LIB_FILES))

cli: cli.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -o $@ $^

cli_r: cli_r.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -o $@ $^

cli_w: cli_w.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -o $@ $^

mem_io: mem_io.o $(LIB_OBJS) $(PUBLIC_LIBS)
	gcc -o $@ $^

config_obj: config_obj.o $(LIB_OBJS) $(PUBLIC_LIBS)
	gcc -o $@ $^

tx_ram_load: tx_ram_load.o $(LIB_OBJS) $(PUBLIC_LIBS)
	gcc -o $@ $^

tx_ram_unload: tx_ram_unload.o $(LIB_OBJS) $(PUBLIC_LIBS)
	gcc -o $@ $^

test1:
	@echo "to test command line interface"
	./cli

test2:
	@echo "to test the command line interface launched from a shell"
	./test_cli.sh

test3:
	@echo "to test configure a group FPGA registers from an input file"
	./mem_io reg_info_file.txt

test4:
	@echo "to test configure any group registers from an input file"
	./config_obj -g 0x10 -s 0x10000 -f config_obj_file.txt

test5:
	@echo "to test fpga writing from reading reg info from a file"
	./cli_w -f cli_w.txt
	@echo "to test fpga writing from reading reg info from command line"
	./cli_w 20 aabbccdd 

test6:
	@echo "to test fpga reading from reading reg info from a file"
	./cli_r -f cli_r.txt
	@echo "to test fpga reading from reading reg info from command line"
	./cli_r 20

test7:
	@echo "to test binary file transfer from sd card to fpga"
	@echo "to test binary file size is greater than allocated fpga segment size"
	./tx_ram_load -g 0x10 -s 0x103 -f tx_simulation_data.bin

	@echo "to test binary file size is less than allocated fpga segment size"
	./tx_ram_load -g 0x10 -s 0x10000 -f tx_simulation_data.bin

test8:
	@echo "to test binary file transfer from fpga to sd card"
	./tx_ram_unload -g 0x10 -s 0x128 -f tx_simulation_data.bin.copy
clean:
	rm -rf *.o $(ALL_EXE) 

#pattern rules
%.o: %.c $(DEPS)
# $<: first prerequisite
	$(CC) -g -c -o $@ $< $(CFLAGS)
