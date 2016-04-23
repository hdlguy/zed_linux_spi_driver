#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "rf_proto_hw.h"
#include "utils.h"

int test_mem_access(const unsigned char *addr)
{
   int i;
   uint32_t val;
   for(i=0;i<16;i++)
   {
      val = i * 256 + 128 + i * 4;
      *(((uint32_t*)addr)+i) = val;
      printf("addr[%02d]=0x%08x,(user space addr=%p)\n",i,*(((uint32_t*)addr)+i),(((uint32_t*)addr)+i));
      sleep(1);
  }
  return 0;
}

int main()
{
    unsigned char *addr;

    addr=phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);

    printf("page size=0x%08X,page size=0x%08lX\n",getpagesize(),sysconf(_SC_PAGE_SIZE));
    // Print out the FPGA_ID value!
    printf("FPGA_ID = 0x%08X from phy/vir addr 0x%08X/%p\n",
           *(uint32_t*)(addr+FPGA_ID),
           PROTO_BASE_ADDRESS+FPGA_ID,
           addr+FPGA_ID);

    mem_write_read_from_file(stdin,addr,0,PROTO_SIZE);
    munmap(addr,PROTO_SIZE);
    return 0;
}
