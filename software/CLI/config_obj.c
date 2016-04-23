#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"
#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "rf_proto_hw.h"
#include "optstring.h"

void usage(char* prog_name)
{
    fprintf(stderr, "Usage: %s [-g seg_offset ] [-s seg_size] [-f config_data_file]\n",prog_name);
}

int main(int argc, char** argv)
{
    unsigned char *virtual_addr;
    uint32_t seg_offset,seg_size;
    FILE* reg_info_file;
    int opt;

    char optstr[64];
    sprintf(optstr,"%c:" "%c:" "%c:",INPUT_FILE_OPT,SEGMENT_OFFSET_OPT,SEGMENT_SIZE_OPT);

    while((opt=getopt(argc,argv,optstr))!=-1)
    {
        switch (opt)
        {
           case SEGMENT_OFFSET_OPT:
               DEBUG("-g %s\n",optarg);
               str_to_hex32_bits(optarg,&seg_offset);
               break;
           case SEGMENT_SIZE_OPT:
               DEBUG("-s %s\n",optarg);
               str_to_hex32_bits(optarg,&seg_size);
               break;
           case INPUT_FILE_OPT:
               DEBUG("-f %s\n",optarg);
               reg_info_file=fopen(optarg,"r");
               break;
           default: /* '?' */
               usage(argv[0]);
               exit(EXIT_FAILURE);
        }
    }

    virtual_addr=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);
    if(virtual_addr==NULL)
    {
       printf("cannot map phy_addr 0x%08x with size 0x%08x to virtual addr with mmap(), errno=%d\n",PROTO_BASE_ADDRESS,seg_size,errno);
       exit(3);
    }

    mem_write_read_from_file(reg_info_file,virtual_addr,seg_offset,PROTO_SIZE);

    munmap(virtual_addr,PROTO_SIZE);

    fclose(reg_info_file);

    return 0;
}
