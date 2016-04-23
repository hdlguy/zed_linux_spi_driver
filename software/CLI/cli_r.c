#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "rf_proto_hw.h"
#include "utils.h"
#include "optstring.h"

void usage(char* prog_name)
{
    fprintf(stderr, "wrong cmd line para. usage: %s [-f config_data_file] or [mem_addr val]\n", prog_name);
}

int main(int argc,char** argv)
{
    unsigned char *addr;
    FILE* reg_info_file=NULL;
    uint32_t mem_addr;
    int opt;
    char optstr[64];

    sprintf(optstr,"%c:" "%c:",INPUT_FILE_OPT,HELP_OPT);
    while((opt=getopt(argc,argv,optstr))!=-1)
    //while((opt=getopt(argc,argv,"f:h:"))!=-1)
    {
        switch (opt)
        {
           case INPUT_FILE_OPT:
               printf("-f %s\n",optarg);
               reg_info_file=fopen(optarg,"r");
               break;
           case HELP_OPT:
           default: 
               usage(argv[0]);
               exit(1);
        }
    }

    if(argc==2&&reg_info_file==NULL)
    {
       if(str_to_hex32_bits(argv[1],&mem_addr)==-1)
       {
          fprintf(stderr,"wrong mem_addr format %s\n",argv[1]);
          exit(2);
       }
    }
    else if(argc!=1&&reg_info_file==NULL)
    {
        usage(argv[0]);
        exit(1);
    }

    addr=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);
    if(addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to virtual addr\n",PROTO_BASE_ADDRESS,PROTO_SIZE);
       exit(3);
    }

//DEBUG("page size=0x%08X,page size=0x%08lX\n",getpagesize(),sysconf(_SC_PAGE_SIZE));
//Print out the FPGA_ID value!
//DEBUG("FPGA_ID=0x%08X in phy/vir addr 0x%08X/%p\n",*(uint32_t*)(addr+FPGA_ID),PROTO_BASE_ADDRESS+FPGA_ID,addr+FPGA_ID);

    if(reg_info_file!=NULL)
       multi_mem_read_from_file(reg_info_file,addr,0,PROTO_SIZE);
    else if(argc==2)
       mem_read(addr,0,mem_addr);
    else
       mem_read_from_file(stdin,addr,0,PROTO_SIZE);

    munmap(addr,PROTO_SIZE);
    return 0;
}
