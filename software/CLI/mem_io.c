#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "rf_proto_hw.h"

int main(int argc, char** argv)
{
    unsigned char *addr;

    FILE* reg_info_file;
    if(argc<2)
    {
       printf("usage: %s reg_info_file\n",argv[0]);
       exit(1);
    }

    reg_info_file=fopen(argv[1],"r");
    if(reg_info_file==NULL)
    {
       printf("can't open reg_info_file %s\n",argv[1]);
       exit(2);
    }

    addr=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);
    if(addr==NULL)
    {
       exit(3);
    }

    // Print out the FPGA_ID value!
    printf("FPGA_ID = 0x%08X from phy/vir addr 0x%08X/%p\n",
           *(uint32_t*)(addr+FPGA_ID),
           PROTO_BASE_ADDRESS+FPGA_ID,
           addr+FPGA_ID);

    mem_write_read_from_file(reg_info_file,addr,0,PROTO_SIZE);
    munmap(addr,PROTO_SIZE);
    fclose(reg_info_file);

    return 0;
}
