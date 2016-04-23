#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "rf_proto_hw.h"
#include "optstring.h"

#define FILE_BUF_SZ 128

void usage(char* prog_name)
{
    fprintf(stderr, "usage: %s reg_base_addr(hex) reg_seg_size(hex) binary_file_name\n",prog_name);
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
               if((binary_file=open(optarg,O_RDONLY))==-1)
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
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to virtual addr\n",PROTO_BASE_ADDRESS,PROTO_SIZE);
       exit(3);
    }

    uint32_t write_index=seg_offset;
    for(;;)
    {
       read_size=read(binary_file,access_buffer,sizeof(access_buffer));
       if(read_size==0)
         break;

       if(write_index-seg_offset+read_size>seg_size) //binary file size is greater than allocated mem size. tested 08/01/2014
         read_size=seg_size-(write_index-seg_offset);

       virtual_mem_range_write(virtual_addr,write_index,access_buffer,read_size);

       write_index+=read_size;

#ifdef DEBUG_MODE
//the *virtual addr is not allowed be displayed with the gdb cmd 'p/x *virtual_addr@write_index'
print_hex_info(__func__,__LINE__,virtual_addr+seg_offset,write_index-seg_offset);
#endif //DEBUG_MODE

       if(write_index-seg_offset>=seg_size)
         break;//binary file size is greater than allocated mem size.
    }

    munmap(virtual_addr,PROTO_SIZE);
    close(binary_file);

    return 0;
}
