#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "ram_mgr.h"
#include "phy_2_vir_addr.h"

void* phy_addr_2_vir_addr(off_t phy_addr,size_t size)
{
   void* vir_addr=NULL;

   int fd = open("/dev/mem",O_RDWR|O_SYNC);
   if(fd < 0)
   {
       printf("Can't open /dev/mem\n");
   }
   else
   {
                  //0 is not NULL
      vir_addr=mmap(0,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,phy_addr);
      if(vir_addr == NULL)
      {
          printf("Can't mmap\n");
      }
      else
      {
          printf("phy_addr 0x%08X is mapped to user addr=%p with size=0x%x bytes\n",(uint32_t)phy_addr,vir_addr,(uint32_t)size);
      }
   }
   return vir_addr;
}

uint32_t virtual_mem_read(void* virtual_address,uint32_t register_offset)
{
   return *(uint32_t*)(virtual_address+register_offset); 
}

void virtual_mem_write(void* virtual_address,uint32_t register_offset,uint32_t value)
{
   *(uint32_t*)(virtual_address+register_offset)=value;
}

void virtual_mem_range_write(uint8_t* virtual_address,uint32_t register_offset,uint8_t* src,uint32_t size)
{
   memcpy(virtual_address+register_offset,src,size);
}

void load_ram(void* fpga_address,ram_name_t ram_name,void* src,int count)
{
   virtual_mem_range_write(fpga_address,ram_name,src,count);
}

void virtual_mem_range_read(uint8_t* dst,uint8_t* virtual_address,uint32_t register_offset,uint32_t size)
{
   memcpy(dst,virtual_address+register_offset,size);
}

void read_ram(void* fpga_address,ram_name_t ram_name,void* dst,int count)
{
   virtual_mem_range_read(dst,fpga_address,ram_name,count);
}
