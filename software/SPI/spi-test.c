#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "common-bit-set.h"
#include "zynq-slcr-regs.h"
#include "zynq-spi-regs.h"
#include "utils.h"
#include "mem-io.h"

#define SLCR_REG_RANGE 0x1000
#define SPI_REG_RANGE  0x1000

uint32_t spi_regs_off[]=
{
 CONFIG_REG,
 INTR_STATUS_REG,
 INTRPT_EN_REG,
 INTRPT_DIS_REG,
 INTRPT_MASK_REG,
 EN_REG,
 DELAY_REG,
 TX_DATA_REG,
 RX_DATA_REG,
 SLAVE_IDLE_COUNT_REG,
 TX_THRES_REG,
 RX_THRES_REG,
 MOD_ID_REG
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
    MIO_PIN_32,
    MIO_PIN_33,
    MIO_PIN_34,
    MIO_PIN_35,
    MIO_PIN_36,
    MIO_PIN_37,
    MIO_PIN_38,
    MIO_PIN_39,
    MIO_PIN_40,
    MIO_PIN_41,
    MIO_PIN_42,
    MIO_PIN_43,
    MIO_PIN_44,
    MIO_PIN_45,
    MIO_PIN_46,
    MIO_PIN_47,
    MIO_PIN_48,
    MIO_PIN_49,
    MIO_PIN_50,
    MIO_PIN_51,
    MIO_PIN_52,
    MIO_PIN_53
};

void config_slcr_for_spi_0(uint8_t* slcr_addr)
{
    write_reg(slcr_addr,MIO_PIN_12,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_11,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_13,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_14,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_15,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_10,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(13));
}

void config_slcr_for_spi_1(uint8_t* slcr_addr)
{
    write_reg(slcr_addr,MIO_PIN_16,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_17,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9));
    write_reg(slcr_addr,MIO_PIN_18,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_19,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_20,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13));
    write_reg(slcr_addr,MIO_PIN_21,SET_BIT_X_TO_1(5)|SET_BIT_X_TO_1(7)|SET_BIT_X_TO_1(9)|SET_BIT_X_TO_1(13));
}

void manual_slave_select_start(uint8_t* spi_base_addr)
{
    uint32_t val;
    write_reg(spi_base_addr,CONFIG_REG,SET_BIT_X_TO_1(10)|SET_BIT_X_TO_1(12)|SET_BIT_X_TO_1(13)|SET_BIT_X_TO_1(14)|SET_BIT_X_TO_1(15));
    write_reg(spi_base_addr,EN_REG,SET_BIT_X_TO_1(0));

    while(1)
    {
       sleep(10);
       val=read_reg(spi_base_addr,EN_REG);

       DEBUG("(*%p)=0x%08x\n",spi_base_addr+EN_REG,val);
       val^=SET_BIT_X_TO_1(0);
       write_reg(spi_base_addr,EN_REG,val);

       val=read_reg(spi_base_addr,CONFIG_REG);
       DEBUG("(*%p)=0x%08x\n",spi_base_addr+CONFIG_REG,val);
       val^=SET_BIT_X_TO_1(11);
       write_reg(spi_base_addr,CONFIG_REG,val);
    }
}

uint32_t CONFIGUED_SPI=SPI_0_BASE_ADDR;

int main(int argc,char** argv)
{
    uint8_t *slcr_addr;
    uint8_t *spi_addr;

    slcr_addr=phy_addr_2_vir_addr(SLCR_REGS_BASE_ADDR,SLCR_REG_RANGE);
    if(slcr_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",SLCR_REGS_BASE_ADDR,SLCR_REG_RANGE);
       exit(3);
    }

    if(argc==2&&argv[1][0]=='0')
       CONFIGUED_SPI=SPI_0_BASE_ADDR;

    if(argc==2&&argv[1][0]=='1')
       CONFIGUED_SPI=SPI_1_BASE_ADDR;

    spi_addr=phy_addr_2_vir_addr(CONFIGUED_SPI,SPI_REG_RANGE);
    if(spi_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",CONFIGUED_SPI,SPI_REG_RANGE);
       exit(3);
    }

    DEBUG("page size=0x%08X,page size=0x%08lX\n",getpagesize(),sysconf(_SC_PAGE_SIZE));

    if(CONFIGUED_SPI==SPI_0_BASE_ADDR)
       config_slcr_for_spi_0(slcr_addr);
    else
       config_slcr_for_spi_1(slcr_addr);

    DEBUG("dump slcr regs\n");
    regs_check(slcr_addr,(uint8_t*)SLCR_REGS_BASE_ADDR,slcr_regs_off,sizeof(slcr_regs_off)/sizeof(*slcr_regs_off));

    DEBUG("dump spi regs\n");
    regs_check(spi_addr,(uint8_t*)CONFIGUED_SPI,spi_regs_off,sizeof(spi_regs_off)/sizeof(*spi_regs_off));

DEBUG("manual_slave_select_start spi\n");
//    manual_slave_select_start(spi_addr);

    munmap(slcr_addr,SLCR_REG_RANGE);
    munmap(spi_addr,SPI_REG_RANGE);

    return 0;
}
