#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "zynq-slcr-regs.h"
#include "zynq-spi-regs.h"
#include "mem-io.h"
#include "../API/rf_proto_hw.h"

#define SLCR_REG_RANGE 0x1000
#define SPI_REG_RANGE  0x1000

#define SPI_BASE_ADDR SPI_1_BASE_ADDR

#define STR_IT(SYMBLE)  #SYMBLE

void mem_info(const char* title,uint8_t* vir_addr,uint32_t phy_addr,int offset)
{
    printf("%s=0x%08X from phy/vir_addr(%p+0x%08x)/(%p+0x%08x)\n",title,*(uint32_t*)(vir_addr+offset),(uint8_t*)phy_addr,offset,vir_addr,offset);

}

int main(int argc,char** argv)
{
    uint8_t *spi_addr;
    uint8_t *fpga_addr;

    spi_addr=phy_addr_2_vir_addr(SPI_BASE_ADDR,SPI_REG_RANGE);
    if(spi_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",SPI_BASE_ADDR,SPI_REG_RANGE);
       exit(-2);
    }


    fpga_addr=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);
    if(fpga_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr\n",PROTO_BASE_ADDRESS,PROTO_BASE_ADDRESS);
       exit(-3);
    }

    // Print out the FPGA_ID value!
    printf("FPGA_ID = 0x%08X from phy/vir addr 0x%08X/%p\n",
           *(uint32_t*)(fpga_addr+FPGA_ID),
           PROTO_BASE_ADDRESS+FPGA_ID,
           fpga_addr+FPGA_ID);

    // Print out the MOD_ID_REG value!
    printf("MOD_ID_REG = 0x%08X from phy/vir addr 0x%08X/%p\n",
           *(uint32_t*)(spi_addr+MOD_ID_REG),
           SPI_BASE_ADDR+MOD_ID_REG,
           spi_addr+MOD_ID_REG);

    mem_info(STR_IT(FPGA_ID),fpga_addr,PROTO_BASE_ADDRESS,FPGA_ID);

    mem_info(STR_IT(CONFIG_REG),spi_addr,SPI_BASE_ADDR,CONFIG_REG);
    mem_info(STR_IT(INTR_STATUS_REG),spi_addr,SPI_BASE_ADDR,INTR_STATUS_REG);
    mem_info(STR_IT(INTRPT_EN_REG),spi_addr,SPI_BASE_ADDR,INTRPT_EN_REG);
    mem_info(STR_IT(INTRPT_DIS_REG),spi_addr,SPI_BASE_ADDR,INTRPT_DIS_REG);
    mem_info(STR_IT(INTRPT_MASK_REG),spi_addr,SPI_BASE_ADDR,INTRPT_MASK_REG);
    mem_info(STR_IT(EN_REG),spi_addr,SPI_BASE_ADDR,EN_REG);
    mem_info(STR_IT(MOD_ID_REG),spi_addr,SPI_BASE_ADDR,MOD_ID_REG);
    mem_info(STR_IT(0x60),spi_addr,SPI_BASE_ADDR,0x60);
    mem_info(STR_IT(0x64),spi_addr,SPI_BASE_ADDR,0x64);

//DELAY_REG
//TX_DATA_REG
//RX_DATA_REG
//SLAVE_IDLE_COUNT_REG
//TX_THRES_REG
//RX_THRES_REG





    munmap(fpga_addr,PROTO_SIZE);
    munmap((void*)SPI_BASE_ADDR,SPI_REG_RANGE);

    return 0;
}
