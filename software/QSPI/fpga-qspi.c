#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "common-bit-set.h"
#include "utils.h"
#include "mem-io.h"
#include "rf_proto_hw.h"
#include "fpga-qspi.h"

#define SPI_DEV_FD uint8_t

static uint16_t write_test_data[]={
                                   0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,
                                   0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa
                                  };

typedef enum
{
  AUTO_TX,
  MANUAL_TX
} TX_MODE_t;

///////////////////////////////////////////////////////////////////////////////////////////////
// driver codes
///////////////////////////////////////////////////////////////////////////////////////////////
SPI_DEV_FD* open_spi_dev_fd()
{
    SPI_DEV_FD* fpga_spi_dev;
    fpga_spi_dev=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);
    if(fpga_spi_dev==NULL)
    {
       fprintf(stderr,"can't mmap 0x%08x with size 0x%08x to vir addr\n",PROTO_BASE_ADDRESS,PROTO_SIZE);
       return NULL;
    }

    DEBUG("page size=0x%08X,page size=0x%08lX\n",getpagesize(),sysconf(_SC_PAGE_SIZE));
    fpga_spi_dev+=AXI_QUAD_SPI_0;
    return fpga_spi_dev;
}
void close_spi_dev(SPI_DEV_FD* fpga_spi_dev)
{
    munmap(fpga_spi_dev,PROTO_SIZE);
}

void check_spi_reg_value(SPI_DEV_FD *fpga_spi_dev)
{
   fprintf(stdout,"check SPI reg values\n");
   fprintf(stdout,"*(SPICR:0x%02x)=0x%08x\n",SPICR,read_reg(fpga_spi_dev,SPICR));
   fprintf(stdout,"*(SPISR:0x%02x)=0x%08x\n",SPISR,read_reg(fpga_spi_dev,SPISR));
   fprintf(stdout,"*(SPIDTR:0x%02x)=0x%08x\n",SPIDTR,read_reg(fpga_spi_dev,SPIDTR));
   fprintf(stdout,"*(SPISSR:0x%02x)=0x%08x\n",SPISSR,read_reg(fpga_spi_dev,SPISSR));
   fprintf(stdout,"*(SPI_TX_FIFO_OCY:0x%02x)=0x%08x\n",SPI_TX_FIFO_OCY,read_reg(fpga_spi_dev,SPI_TX_FIFO_OCY));
   fprintf(stdout,"*(SPI_RX_FIFO_OCY:0x%02x)=0x%08x\n",SPI_RX_FIFO_OCY,read_reg(fpga_spi_dev,SPI_RX_FIFO_OCY));
   fprintf(stdout,"*(DGIER:0x%02x)=0x%08x\n",DGIER,read_reg(fpga_spi_dev,DGIER));
   fprintf(stdout,"*(IPISR:0x%02x)=0x%08x\n",IPISR,read_reg(fpga_spi_dev,IPISR));
   fprintf(stdout,"*(IPIER:0x%02x)=0x%08x\n",IPIER,read_reg(fpga_spi_dev,IPIER));
}

void configure_auto_spi_tx(SPI_DEV_FD *fpga_spi_dev)
{
    uint32_t val;
    write_reg(fpga_spi_dev,SRR,SSR_RESET);
    check_spi_reg_value(fpga_spi_dev);

    val=read_reg(fpga_spi_dev,SPICR);
//    val|=SPICR_LOOP;
    val|=SPICR_SPE;
    val|=SPICR_MASTER;
//    val|=SPICR_LSB_FIRST;
    val&=~SPICR_MASTER_TRANSACTION_INHIBIT;
    val&=~SPICR_MANUAL_SLAVE_SELECT_ASSERTION_ENABLE;
    write_reg(fpga_spi_dev,SPICR,val);
}

void configure_manual_spi_tx(SPI_DEV_FD *fpga_spi_dev)
{
    configure_auto_spi_tx(fpga_spi_dev);
//    uint32_t val;
//    val=read_reg(fpga_spi_dev,SPICR);
//    val|=SPICR_MANUAL_SLAVE_SELECT_ASSERTION_ENABLE; //WHY MUST BE UNSET??? 09/12/2014
//    write_reg(fpga_spi_dev,SPICR,val);
}

void slave_assertion(SPI_DEV_FD *fpga_spi_dev,uint32_t slave)
{
    uint32_t val;
    val=read_reg(fpga_spi_dev,SPISSR);
    val&=SET_BIT_X_TO_0(slave);
    write_reg(fpga_spi_dev,SPISSR,val);
}

void slave_deassertion(SPI_DEV_FD *fpga_spi_dev,uint32_t slave)
{
    uint32_t val;
    val=read_reg(fpga_spi_dev,SPISSR);
    val|=SET_BIT_X_TO_1(slave);
    write_reg(fpga_spi_dev,SPISSR,val);
}

static int transmit_one_word(SPI_DEV_FD *fpga_spi_dev,uint32_t* value)
{
  int result=0;
  if((read_reg(fpga_spi_dev,SPISR)&SPISR_TX_FULL)==0)
  {
     write_reg(fpga_spi_dev,SPIDTR,*value);
     //DEBUG("write 0x%04x to SPIDTR(0x%02x) successfully\n",*value&TRANSFER_WIDTH_MASK,SPIDTR);
     result=1;
  }
  else
  {
     ERR_LOG("write 0x%04x to SPIDTR(0x%08x) failed due to TX FIFO full\n",*value,SPIDTR);
  }

  return result;
}

static int receive_one_word(SPI_DEV_FD *fpga_spi_dev,uint32_t* value)
{
  int result=0;
  if((read_reg(fpga_spi_dev,SPISR)&SPISR_RX_EMPTY)==0)
  {
     *value=read_reg(fpga_spi_dev,SPIDRR);
     result=1;
  }
  else
  {
     ERR_LOG("read failed due to RX FIFO empty\n");
  }
  return result;
}

int auto_write(SPI_DEV_FD *fpga_spi_dev,uint16_t* data,uint32_t data_size,uint32_t slave)
{
   int i;
   for(i=0;i<data_size;i++)
   {
      int result;
      result=transmit_one_word(fpga_spi_dev,(uint32_t*)(data+i));

      if(result==0)
        break;
   }
}

int manual_write(SPI_DEV_FD *fpga_spi_dev,uint16_t* data,uint32_t data_size,uint32_t slave)
{
   int i;
   slave_assertion(fpga_spi_dev,slave);
   for(i=0;i<data_size;i++)
   {
      DEBUG("write SPIDTR 0x%04x\n",data[i]);
      write_reg(fpga_spi_dev,SPIDTR,data[i]);
   }
   usleep(1000); //TBD put effective value;
   slave_deassertion(fpga_spi_dev,slave);
   return 0;
}

//not working, if no data is injected to TX FIFO
void test_slave_select(SPI_DEV_FD *fpga_spi_dev,uint32_t slave)
{
    uint32_t val;

    while(1)
    {
       val=read_reg(fpga_spi_dev,SPISSR);
       val^=SET_BIT_X_TO_1(slave);
       write_reg(fpga_spi_dev,SPISSR,val);
   fprintf(stdout,"*(SPICR:0x%08x)=0x%08x\n",SPICR,read_reg(fpga_spi_dev,SPICR));
   fprintf(stdout,"*(SPISR:0x%08x)=0x%08x\n",SPISR,read_reg(fpga_spi_dev,SPISR));
   fprintf(stdout,"*(SPISSR:0x%08x)=0x%08x\n\n",SPISSR,read_reg(fpga_spi_dev,SPISSR));
       usleep(1000000);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// test codes
///////////////////////////////////////////////////////////////////////////////////////////////
void test_auto_tx(SPI_DEV_FD *fpga_spi_dev)
{
    uint32_t i;
    for(i=0;;i++)
    {
       printf("test_auto_tx %u\n",i);
       auto_write(fpga_spi_dev,write_test_data,sizeof(write_test_data)/sizeof(*write_test_data),0);
       usleep(1000000);
    }
}

void test_manual_tx(SPI_DEV_FD *fpga_spi_dev)
{
    uint32_t i;
    for(i=0;;i++)
    {
       printf("test_manual_tx %u\n",i);
       manual_write(fpga_spi_dev,write_test_data,sizeof(write_test_data)/sizeof(*write_test_data),0);
       usleep(1000000);
    }
}

void test_local_loopback(SPI_DEV_FD *fpga_spi_dev,uint32_t slave,TX_MODE_t tx_mode)
{
    uint32_t i;
    for(i=0;;i++)
    {
       DEBUG("loopback test %u\n",i+1);
       DEBUG("pre writing SPIDTX, *(SPISR:0x%02x)=0x%04x\n",SPISR,read_reg(fpga_spi_dev,SPISR));

       if(tx_mode==AUTO_TX)
         auto_write(fpga_spi_dev,write_test_data,sizeof(write_test_data)/sizeof(*write_test_data),slave);
       else
         manual_write(fpga_spi_dev,write_test_data,sizeof(write_test_data)/sizeof(*write_test_data),slave);

       DEBUG("post writing SPIDTX, *(SPISR:0x%02x)=0x%04x\n",SPISR,read_reg(fpga_spi_dev,SPISR));
       while(1)
       {
          uint32_t value;
          int read_result=0;
          DEBUG("pre reading SPIDRR, *(SPI_RX_FIFO_OCY:0x%02x)=0x%02x\n",
          SPI_RX_FIFO_OCY,read_reg(fpga_spi_dev,SPI_RX_FIFO_OCY));

          read_result=receive_one_word(fpga_spi_dev,&value);
          if(read_result==0)
             break;

          DEBUG("LOOPBACK READ SPIDRR=0x%02x\n",value);

          DEBUG("post reading SPIDRR, *(SPISR:0x%02x)=0x%04x\n",
          SPISR,read_reg(fpga_spi_dev,SPISR));

          DEBUG("post reading SPIDRR, *(SPI_RX_FIFO_OCY:0x%02x)=0x%02x\n\n",
          SPI_RX_FIFO_OCY,read_reg(fpga_spi_dev,SPI_RX_FIFO_OCY));
       }
       usleep(1000000);
    }
}

void fill_in_lime_regs(SPI_DEV_FD *fpga_spi_dev)
{
    uint8_t i;
    uint32_t val=0;
    for(i=0;i<128;i++)
    {
       //val=127-i;
       val=i;
       val|=i<<8;
       val|=0x8000;
       printf("filling addr 0x%02x=0x%02x, write spi 0x%04x to lime\n",i,(uint8_t)val,val);
       transmit_one_word(fpga_spi_dev,&val);
       usleep(100);
    }
    usleep(1000);
}

int lime_reg_read(SPI_DEV_FD *fpga_spi_dev,uint8_t lime_reg,uint32_t *val)
{
    uint32_t read_addr=lime_reg<<8;
    transmit_one_word(fpga_spi_dev,&read_addr);
    usleep(100);
    if(receive_one_word(fpga_spi_dev,val)==1)
    {
//       DEBUG("read lime reg 0x%02x=0x%02x\n",lime_reg,*val);
    }
    else
    {
       ERR_LOG("read lime reg 0x%02x failed\n",lime_reg);
    }
}

void dump_lime_regs(SPI_DEV_FD *fpga_spi_dev)
{
   uint32_t val;
   uint32_t i;
   for(i=0;i<128;i++)
   {
      lime_reg_read(fpga_spi_dev,i,&val);
      DEBUG("read lime reg 0x%02x=0x%02x\n",i,val); //TBD why "read lime reg 0x00=0x7f00"? 09/16/14. we only care last 8 bits
   }
}

void test_talk_with_slave(SPI_DEV_FD *fpga_spi_dev,uint8_t start_lime_reg)
{
   fill_in_lime_regs(fpga_spi_dev);
   dump_lime_regs(fpga_spi_dev);

   while(1)
   {
      uint32_t val;
      lime_reg_read(fpga_spi_dev,start_lime_reg,&val);
      DEBUG("read lime reg 0x%02x=0x%02x\n",start_lime_reg,val);
      usleep(1000000);
      ++start_lime_reg;
      start_lime_reg%=128;
   }
}

int main(int argc,char** argv)
{
   uint8_t lime_reg=7;

   if(argc==2)
   {
     lime_reg=atoi(argv[1]);
     if(lime_reg>127||lime_reg<0)
     {
        ERR_LOG("usage %s lime_reg_num(between 0 and 127)\n",argv[0]);
        exit(1);
     }
   }
 
   SPI_DEV_FD* fpga_spi_dev=open_spi_dev_fd();
   configure_auto_spi_tx(fpga_spi_dev);
   slave_assertion(fpga_spi_dev,0);
   test_talk_with_slave(fpga_spi_dev,lime_reg);

   //test_local_loopback(fpga_spi_dev,0,AUTO_TX);

//    configure_manual_spi_tx(fpga_spi_dev);
//    test_local_loopback(fpga_spi_dev,0,MANUAL_TX);

   close_spi_dev(fpga_spi_dev);

   return 0;
}
