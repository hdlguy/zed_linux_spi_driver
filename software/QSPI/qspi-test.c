#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "common-bit-set.h"
#include "zynq-slcr-regs.h"
#include "zynq-qspi-regs.h"
#include "zynq-gpio-regs.h"
#include "utils.h"
#include "mem-io.h"

#define SLCR_REG_RANGE 0x1000
#define QSPI_REG_RANGE 0x1000
#define GPIO_REG_RANGE 0x1000

uint32_t qspi_regs_off[]=
{
   ZYNQ_QSPI_CONFIG_OFFSET,
   ZYNQ_QSPI_STATUS_OFFSET,
   //ZYNQ_QSPI_IEN_OFFSET,   //WO
   //ZYNQ_QSPI_IDIS_OFFSET,  //WO
   ZYNQ_QSPI_IMASK_OFFSET,
   ZYNQ_QSPI_ENABLE_OFFSET,
   ZYNQ_QSPI_DELAY_OFFSET,
   //ZYNQ_QSPI_TXD_00_00_OFFSET,   //WO
   //ZYNQ_QSPI_TXD_00_01_OFFSET,   //WO
   //ZYNQ_QSPI_TXD_00_10_OFFSET,   //WO
   //ZYNQ_QSPI_TXD_00_11_OFFSET,   //WO
//   ZYNQ_QSPI_RXD_OFFSET, //TBD why can't read it 08/13/2014
   ZYNQ_QSPI_SIC_OFFSET,
   ZYNQ_QSPI_TX_THRESH_OFFSET,
   ZYNQ_QSPI_RX_THRESH_OFFSET,
   ZYNQ_QSPI_GPIO_OFFSET,
   ZYNQ_QSPI_LINEAR_CFG_OFFSET,
   ZYNQ_QSPI_MOD_ID_OFFSET
};

uint32_t slcr_regs_off[]=
{
   MIO_PIN_00,
   MIO_PIN_01,
   MIO_PIN_02,
   MIO_PIN_03,
   MIO_PIN_04,
   MIO_PIN_05,
   MIO_PIN_06,
   MIO_PIN_07,
   MIO_PIN_08,
   MIO_PIN_09,
   MIO_PIN_10,
   MIO_PIN_11,
   MIO_PIN_12,
   MIO_PIN_13,
   MIO_PIN_14,
   MIO_PIN_15,
   MIO_PIN_16,
   MIO_PIN_17,
   MIO_PIN_18,
   MIO_PIN_19,
   MIO_PIN_20,
   MIO_PIN_21,
   MIO_PIN_22,
   MIO_PIN_23,
   MIO_PIN_24,
   MIO_PIN_25,
   MIO_PIN_26,
   MIO_PIN_27,
   MIO_PIN_28,
   MIO_PIN_29,
   MIO_PIN_30,
   MIO_PIN_31,
   MIO_PIN_32
};

void config_slcr(uint8_t* slcr_addr)
{
    DEBUG("dump slcr regs\n");
    regs_check(slcr_addr,(uint8_t*)SLCR_REGS_BASE_ADDR,slcr_regs_off,sizeof(slcr_regs_off)/sizeof(*slcr_regs_off));

    write_reg(slcr_addr,MIO_PIN_00,0x1302);
    write_reg(slcr_addr,MIO_PIN_09,0x302);
    write_reg(slcr_addr,MIO_PIN_10,0x302);
    write_reg(slcr_addr,MIO_PIN_11,0x302);
    write_reg(slcr_addr,MIO_PIN_12,0x302);
    write_reg(slcr_addr,MIO_PIN_13,0x302);
    write_reg(slcr_addr,MIO_PIN_08,0x302);
#if 0
    write_reg(slcr_addr,MIO_PIN_00,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12));
    write_reg(slcr_addr,MIO_PIN_08,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_09,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_10,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_11,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_12,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_13,SET_BIT_X_TO_1(1)|SET_BIT_X_TO_1(8)|SET_BIT_X_TO_1(9));
#endif
}

void test_qspi_io(uint8_t* qspi_addr)
{
    write_reg(qspi_addr,ZYNQ_QSPI_CONFIG_OFFSET,SET_BIT_X_TO_1(14)|SET_BIT_X_TO_1(15));
    write_reg(qspi_addr,ZYNQ_QSPI_LINEAR_CFG_OFFSET,SET_BIT_X_TO_1(29)|SET_BIT_X_TO_1(30));
    write_reg(qspi_addr,ZYNQ_QSPI_ENABLE_OFFSET,SET_BIT_X_TO_1(0));

    while(1)
    {
       uint32_t val;
       sleep(1);
       val=read_reg(qspi_addr,ZYNQ_QSPI_ENABLE_OFFSET);
       val^=SET_BIT_X_TO_1(0);
       write_reg(qspi_addr,ZYNQ_QSPI_ENABLE_OFFSET,val);

       val=read_reg(qspi_addr,ZYNQ_QSPI_CONFIG_OFFSET);
       val^=SET_BIT_X_TO_1(10);
       write_reg(qspi_addr,ZYNQ_QSPI_CONFIG_OFFSET,val);
    }
}

//MIO_PIN_13 is connected to pin_1 on J5 on the MicroZed board
void test_gpio(uint8_t* gpio_addr,uint32_t mio_pin)
{
    uint32_t tmp_val;
    tmp_val=read_reg(gpio_addr,DIRM_0);
    tmp_val|=SET_BIT_X_TO_1(mio_pin);
    write_reg(gpio_addr,DIRM_0,tmp_val);

    tmp_val=read_reg(gpio_addr,OEN_0);
    tmp_val|=SET_BIT_X_TO_1(mio_pin);
    write_reg(gpio_addr,OEN_0,tmp_val);
  
    while(1)
    {
       tmp_val=read_reg(gpio_addr,DATA_0);
       tmp_val^=SET_BIT_X_TO_1(mio_pin);
       DEBUG("*(%p)=0x%08x\n",(void*)(0xE000A000)+DATA_0,tmp_val);
       write_reg(gpio_addr,DATA_0,tmp_val);
       usleep(1000);
    }
}

int main(int argc,char** argv)
{
    uint8_t *slcr_addr;
    uint8_t *qspi_addr;
    uint8_t *gpio_addr;

    slcr_addr=phy_addr_2_vir_addr(SLCR_REGS_BASE_ADDR,SLCR_REG_RANGE);
    if(slcr_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",SLCR_REGS_BASE_ADDR,SLCR_REG_RANGE);
       exit(3);
    }

    qspi_addr=phy_addr_2_vir_addr(QSPI_BASE_ADDR,QSPI_REG_RANGE);
    if(qspi_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",QSPI_BASE_ADDR,QSPI_REG_RANGE);
       exit(3);
    }

    gpio_addr=phy_addr_2_vir_addr(GPIO_BASE_ADDR,GPIO_REG_RANGE);
    if(qspi_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",GPIO_BASE_ADDR,GPIO_REG_RANGE);
       exit(3);
    }

    DEBUG("page size=0x%08X,page size=0x%08lX\n",getpagesize(),sysconf(_SC_PAGE_SIZE));

    DEBUG("dump slcr regs\n");
    regs_check(slcr_addr,(uint8_t*)SLCR_REGS_BASE_ADDR,slcr_regs_off,sizeof(slcr_regs_off)/sizeof(*slcr_regs_off));

    DEBUG("dump qspi regs\n");
    regs_check(qspi_addr,(uint8_t*)QSPI_BASE_ADDR,qspi_regs_off,sizeof(qspi_regs_off)/sizeof(*qspi_regs_off));

//    config_slcr(slcr_addr);
//    test_qspi_io(qspi_addr);
    test_gpio(gpio_addr,13);

    munmap(slcr_addr,SLCR_REG_RANGE);
    munmap(qspi_addr,QSPI_REG_RANGE);
    munmap(gpio_addr,GPIO_REG_RANGE);

    return 0;
}
