ALL_EXE = qspi-test fpga-qspi
all: $(ALL_EXE) 

CC=gcc
CFLAGS=-I. -I../include -I../API
DEPS = Makefile

PUBLIC_LIBS = ../API/mem-io.o 
LIB_FILES =
LIB_OBJS := $(subst .c,.o,$(LIB_FILES))

qspi-test: qspi-test.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

fpga-qspi: fpga-qspi.o $(LIB_OBJS) $(PUBLIC_LIBS) fpga-qspi.h
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

test: qspi-test
	./qspi-test

t-fpga: fpga-qspi
	./fpga-qspi 12

clean:
	rm -rf *.o $(ALL_EXE) 

#pattern rules
%.o: %.c $(DEPS)
# $<: first prerequisite
	$(CC) -g -c -o $@ $< $(CFLAGS)
