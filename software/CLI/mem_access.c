#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "ram_mgr.h"
#include "phy_2_vir_addr.h"
#include "mem_access.h"
#include "utils.h"

#define TRUE    1
#define FALSE   0

#define READ_INPUT_NUM 2
#define WRITE_INPUT_NUM 3

static void clean_input_buffer(FILE* input_file)
{
   while(fgetc(input_file)!='\n');
#if 0
   int chr;
   while(1)
   {
    chr=fgetc(input_file);
    if(chr=='\n')
       break;
   }
#endif
}

//return:
//       0: check failed
//       1: check passed
int static check_fscanf_result
(FILE* input_file,uint32_t seg_offset,uint32_t vir_mem_size,uint32_t mem_addr,int input_num,int expect_num)
{
   if(input_num==EOF)
   {
      if(errno==0)
      {
         DEBUG("reaching the end of input %s file\n",expect_num==READ_INPUT_NUM?"read":"write");
      }
      else
      {
         DEBUG("reading input file error, errno=%d\n",errno);
      }
   }
   else if(seg_offset+mem_addr+sizeof(uint32_t)>=vir_mem_size)
   {
      fprintf(stderr,"seg_offset+mem_addr+sizeof(uint32_t) exceeds limitation 0x%08x\n",vir_mem_size);
   }
   else if(input_num!=expect_num)
   {
      fprintf(stderr,"wrong data format\n");
      clean_input_buffer(input_file); 
   }
   return input_num==expect_num;
}

void mem_read(unsigned char *vir_addr,uint32_t seg_offset,uint32_t mem_addr)
{
   fprintf(stdout,"0X%08X\n",virtual_mem_read(vir_addr+seg_offset,mem_addr));
   DEBUG("read 0X%08X from mem_addr=0x%08x/vir_addr=%p\n",
          virtual_mem_read(vir_addr+seg_offset,mem_addr),
          seg_offset+mem_addr,
          vir_addr+seg_offset+mem_addr);
}

// return: 2 is right
//         less than 2, format is not right
//         EOF and errno=0, reaching the end of file
int mem_read_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t vir_mem_size)
{
   uint32_t mem_addr;
   char new_line; //to skip the new line for next run
   int i;
   i=fscanf(input_file,"%x%c",&mem_addr,&new_line);
   if(check_fscanf_result(input_file,seg_offset,vir_mem_size,mem_addr,i,READ_INPUT_NUM)==TRUE)
   {
      mem_read(vir_addr,seg_offset,mem_addr);
   }
   return i;
}

void mem_write(unsigned char *vir_addr,uint32_t seg_offset,uint32_t mem_addr,uint32_t val)
{
      DEBUG("write 0x%08x to mem_addr 0x%08x\n",val,seg_offset+mem_addr);
      virtual_mem_write(vir_addr+seg_offset,mem_addr,val);
      DEBUG("write 0X%08X to mem_addr 0x%08x/vir_addr=%p\n",
             virtual_mem_read(vir_addr+seg_offset,mem_addr),
             seg_offset+mem_addr,
             vir_addr+seg_offset+mem_addr);
}

// return: 3 is right
//         less than 3, format is not right
//         EOF, check errors 
int mem_write_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t vir_mem_size)
{
   uint32_t val;
   uint32_t mem_addr;
   char new_line; //to skip the new line for next run
   int i=fscanf(input_file,"%x %x%c",&mem_addr,&val,&new_line);
   if(check_fscanf_result(input_file,seg_offset,vir_mem_size,mem_addr,i,WRITE_INPUT_NUM)==TRUE)
   {
      mem_write(vir_addr,seg_offset,mem_addr,val);
   }
   return i;
}

int multi_mem_read_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t vir_mem_size)
{
   while(1)
   {
      if(mem_read_from_file(input_file,vir_addr,seg_offset,vir_mem_size)==EOF)
         break;
   }
   return 0;
}

int multi_mem_write_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t vir_mem_size)
{
   while(1)
   {
      if(mem_write_from_file(input_file,vir_addr,seg_offset,vir_mem_size)==EOF)
        break;
   }
   return 0;
}

int mem_write_read_from_file(FILE* input_file,unsigned char *vir_addr,uint32_t seg_offset,uint32_t seg_size)
{
   int cmd;
   while(1)
   {
      char new_line; //to skip the new line for next run
//sleep(1);
      if(input_file==stdin)
      {
        fprintf(stdout,"\nplease input command: [w phy_addr_off val with hex], [r phy_addr_off with hex] or [q to exit]\n");
      }

proc_next_line: //for goto, logic is simpler by using goto

      //i=fscanf(input_file,"%*[ \n\t]%c",&cmd); not woring well
      cmd=fgetc(input_file);

      if(cmd=='w'||cmd=='W')
      {
         mem_write_from_file(input_file,vir_addr,seg_offset,seg_size);
      }
      else if(cmd=='r'||cmd=='R')
      {
         mem_read_from_file(input_file,vir_addr,seg_offset,seg_size);
      }
      else if(cmd=='q'||cmd=='Q')
      {
         if(input_file==stdin)
         {
            DEBUG("cmd=%c, exit\n",cmd);
         }
         break;
      }
      else if(cmd=='#')
      {
         //int in_char;
         //DEBUG("COMMENT LINE\n");
         //putchar('#');
         //while((in_char=fgetc(input_file))!='\n') putchar(in_char);putchar('\n'); //clean the input buffer
         clean_input_buffer(input_file);
         goto proc_next_line;//continue;
      }
      else if(isspace(cmd))
      {
         //DEBUG("empty line\n");
         goto proc_next_line;//continue;
      }
      else if(cmd==EOF)
      {
         if(input_file==stdin)
         {
            DEBUG("on end of file\n");
         }
         break;
      }
      else
      {
         fprintf(stderr,"invalid cmd 0x%x\n",cmd);
         break;
      }
   } 
   return 0;
}
