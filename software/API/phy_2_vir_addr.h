#ifndef __PHY_2_VIR_ADDR_H__
#define __PHY_2_VIR_ADDR_H__

void* phy_addr_2_vir_addr(off_t phy_addr,size_t size);
#define get_fpga_virtual address() phy_addr_2_vir_addr(PROTO_BASE_ADDRESS,PROTO_SIZE);

uint32_t virtual_mem_read(void* virtual_address,uint32_t register_offset);
void virtual_mem_write(void* virtual_address,uint32_t register_offset,uint32_t value);

void virtual_mem_range_write(uint8_t* virtual_address,uint32_t register_offset,uint8_t* src,uint32_t size);
void virtual_mem_range_read(uint8_t* dst,uint8_t* virtual_address,uint32_t register_offset,uint32_t size);

#define FPGA_reg_write          virtual_mem_write
#define FPGA_reg_read           virtual_mem_read

#define max_SPI_write           virtual_mem_write
#define max_SPI_read            virtual_mem_read

#define PA_SPI_write            virtual_mem_write
#define PA_SPI_read             virtual_mem_read

#define poke_fpga               virtual_mem_write
#define peek_fpga               virtual_mem_read

void load_ram(void* fpga_address,ram_name_t ram_name,void* src,int count);
void read_ram(void* fpga_address,ram_name_t ram_name,void* dst,int count);

#endif// __PHY_2_VIR_ADDR_H__
