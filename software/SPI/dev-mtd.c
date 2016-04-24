#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "common-bit-set.h"
#include "zynq-slcr-regs.h"
#include "zynq-spi-regs.h"
#include "utils.h"
#include "mem-io.h"
#include "utils.h"

#define SLCR_REG_RANGE 0x1000
#define SPI_REG_RANGE  0x1000

#define DEVICE "/dev/mtd0"

uint32_t slcr_mio_regs_off[]=
{
    MIO_PIN_10,
    MIO_PIN_11,
    MIO_PIN_12,
    MIO_PIN_13,
    MIO_PIN_14,
    MIO_PIN_15,
};

int test_dev_file(char* dev_name)
{
   int fd=0;
   unsigned char buf[256];
   int result;
   //fd = open("/dev/spidev0.2",O_RDWR);

   memset(buf,0x5a,sizeof(buf));
   errno=0;
   fd = open(dev_name,O_RDWR);
   if(fd < 0)
   {
       fprintf(stderr,"Can't open \"%s\", fd=%d, errno=%d\n",dev_name,fd,errno);
       return -1;
   }
   else
   {
       fprintf(stderr,"\"%s\" is opend, fd=%d\n",dev_name,fd);
   }

   print_hex_info(__func__,__LINE__,buf,sizeof buf);
   result=write(fd,buf,sizeof buf);
   printf("write return %d\n",result);

   memset(buf,0x00,sizeof(buf));
   result=read(fd,buf,sizeof buf); 
   printf("read return %d\n",result);
   print_hex_info(__func__,__LINE__,buf,sizeof buf);
   close(fd);
}

void check_mio_set(const uint8_t* vir_addr, const uint8_t* phy_addr,uint32_t* regs_off,uint32_t regs_num)
{
   #define MIO_PIN0_OFFSET 0x700
   int i;
   for(i=0;i<regs_num;i++)
      printf("MIO_PIN_%02d="
             "0x%08x,"
             "(phy_addr=%p,vir_addr=%p)\n",
              (regs_off[i]-MIO_PIN0_OFFSET)/4,
             *((uint32_t*)(vir_addr+regs_off[i])),
             phy_addr+regs_off[i],
             vir_addr+regs_off[i]);

}

void set_mio_for_spi_1(uint8_t* zynq_slcr_base)
{
   *((uint32_t*)(zynq_slcr_base+MIO_PIN_12))=0x22a0;
   *((uint32_t*)(zynq_slcr_base+MIO_PIN_11))=0x2a0;
   *((uint32_t*)(zynq_slcr_base+MIO_PIN_13))=0x32a0;
   *((uint32_t*)(zynq_slcr_base+MIO_PIN_14))=0x32a0;
   *((uint32_t*)(zynq_slcr_base+MIO_PIN_15))=0x32a0;
   *((uint32_t*)(zynq_slcr_base+MIO_PIN_10))=0x22a0;
}

int main(int argc,char** argv)
{
   uint8_t *slcr_addr;
   uint8_t *spi_addr;
   char dev_file[16];
   int i;

//   for(i=0;i<5;i++)
//   {//only mtd0 can be opened if spi1 is NOT configured with slcr MIO_PIN_10 through MIO_PIN_15
//      sprintf(dev_file,"/dev/mtd%d",i);
//      test_dev_file(dev_file);
//   }

//   for(i=0;i<5;i++)
//   {
//      sprintf(dev_file,"/dev/mtd%dro",i);
//      test_dev_file(dev_file);
//   }

   test_dev_file(DEVICE);

   slcr_addr=phy_addr_2_vir_addr(SLCR_REGS_BASE_ADDR,SLCR_REG_RANGE);
   if(slcr_addr==NULL)
   {
      fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",SLCR_REGS_BASE_ADDR,SLCR_REG_RANGE);
      exit(3);
   }

//   DEBUG("dump slcr regs\n");
//   check_mio_set(slcr_addr,(uint8_t*)SLCR_REGS_BASE_ADDR,slcr_mio_regs_off,sizeof(slcr_mio_regs_off)/sizeof(*slcr_mio_regs_off));

//   set_mio_for_spi_1(slcr_addr);
//   check_mio_set(slcr_addr,(uint8_t*)SLCR_REGS_BASE_ADDR,slcr_mio_regs_off,sizeof(slcr_mio_regs_off)/sizeof(*slcr_mio_regs_off));
   //test_dev_file(DEVICE);
   munmap(slcr_addr,SLCR_REG_RANGE);
   return 0;
}