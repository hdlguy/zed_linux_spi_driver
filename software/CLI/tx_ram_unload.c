#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "rf_proto_hw.h"
#include "optstring.h"

#define FILE_BUF_SZ 128

void usage(char* prog_name)
{
    fprintf(stderr, "usage: %s reg_base_addr(hex) reg_seg_size(hex) binary_file\n",prog_name);
}

int main(int argc, char** argv)
{
    unsigned char *virtual_addr;
    uint32_t seg_offset=0,seg_size=PROTO_SIZE;
    int binary_file;
    uint8_t access_buffer[FILE_BUF_SZ];

    size_t read_size;
    int result;
    int opt;
    char optstr[64];

    sprintf(optstr,"%c:" "%c:" "%c:",INPUT_FILE_OPT,SEGMENT_OFFSET_OPT,SEGMENT_SIZE_OPT);
    while((opt=getopt(argc,argv,optstr))!=-1)
    {
        switch (opt)
        {
           case SEGMENT_OFFSET_OPT:
               DEBUG("-g %s\n",optarg);
               if(str_to_hex32_bits(optarg,&seg_offset)==-1)
               {
                  usage(argv[0]);
                  exit(EXIT_FAILURE);
               }
               break;
           case SEGMENT_SIZE_OPT:
               DEBUG("-s %s\n",optarg);
               if(str_to_hex32_bits(optarg,&seg_size)==-1)
               {
                  usage(argv[0]);
                  exit(EXIT_FAILURE);
               }
               break;
           case INPUT_FILE_OPT:
               DEBUG("-f %s\n",optarg);
               if((binary_file=open(optarg,O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH))==-1)
               {
                  fprintf(stderr,"cannot open image bin file %s\n",optarg);
                  usage(argv[0]);
                  exit(EXIT_FAILURE);
               }
               break;
           default: /* '?' */
               usage(argv[0]);
               exit(EXIT_FAILURE);
        }
    }

    virtual_addr=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);
    if(virtual_addr==NULL)
    {
       fprintf(stderr,"can't mmap() phy_addr 0x%08x with size 0x%08x to virtual addr\n",PROTO_BASE_ADDRESS,PROTO_SIZE);
       exit(3);
    }

    uint32_t read_index=0;
    for(;;)
    {
       //08/01/2014 tested both the sizeof(access_buffer) greater/less than seg_size
       if(seg_offset+read_index+sizeof(access_buffer)>=seg_size)
          read_size=seg_size-read_index;
       else
          read_size=sizeof(access_buffer);

       virtual_mem_range_read(access_buffer,virtual_addr+seg_offset,read_index,read_size);
       result=write(binary_file,access_buffer,read_size);
       if(result!=read_size)
       {
          fprintf(stderr,"write() error %s\n",strerror(errno));
          break;
       }
       read_index+=read_size;

       if(read_index>=seg_size)
          break;
    }

    munmap(virtual_addr,PROTO_SIZE);
    close(binary_file);

    return 0;
}
