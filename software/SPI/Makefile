ALL_EXE = dev-mtd dev-spi simple-test spi-test
all: $(ALL_EXE) 

CC=gcc
CFLAGS=-I. -I../include  -I../API
DEPS = Makefile

PUBLIC_LIBS = ../API/utils.o ../API/mem-io.o
LIB_FILES =
#LIB_FILES = mem-io.c

LIB_OBJS := $(subst .c,.o,$(LIB_FILES))

dev-spi: dev-spi.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

dev-mtd: dev-mtd.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

simple-test: simple-test.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

spi-test: spi-test.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

test: simple-test
	./simple-test

test0: spi-test
	echo "test spi_0"
	./spi-test 0

test1: spi-test
	echo "test spi_1"
	./spi-test 1

test-mtd: dev-mtd
	./dev-mtd

test-spi: dev-spi
	./dev-spi /dev/spidev32766 

clean:
	rm -rf *.o $(ALL_EXE) 

#pattern rules
%.o: %.c $(DEPS)
# $<: first prerequisite
	$(CC) -g -c -o $@ $< $(CFLAGS)
