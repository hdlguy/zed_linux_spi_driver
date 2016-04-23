#ifndef __MEM_ACCESS_H__
#define __MEM_ACCESS_H__

void mem_read(unsigned char *vir_addr,uint32_t seg_offset,uint32_t mem_addr);
void mem_write(unsigned char *vir_addr,uint32_t seg_offset,uint32_t mem_addr,uint32_t val);

int mem_read_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t phy_base_addr,uint32_t seg_size);
int mem_write_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t phy_base_addr,uint32_t seg_size);

int multi_mem_read_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t seg_size);
int multi_mem_write_read_from_file(FILE* input_file,unsigned char *addr,uint32_t seg_base_addr,uint32_t seg_size);

int mem_write_read_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t seg_size);
#endif// __MEM_ACCESS_H__
