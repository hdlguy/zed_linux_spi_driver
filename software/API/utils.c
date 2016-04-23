#include <stdio.h>
#include <stdint.h>

#include "utils.h"

void print_hex_info(const char* title,int calling_line_num,const unsigned char* hex_info,unsigned int info_size)
{
   #define COLUMN_NUM 16
   unsigned int i;
   printf("from \"%s\"@%d: size=%d\n",title,calling_line_num,info_size);

   for(i=0;i<info_size;i++)
   {
      if(i%COLUMN_NUM==0)
        printf("<%03d>",i/COLUMN_NUM+1); //print line num
      printf("%02X%c",hex_info[i],(i==(info_size-1)||(i+1)%COLUMN_NUM==0)?'\n':',');
   }
}

int str_to_hex32_bits(const char* str,uint32_t* value)
{
   if(sscanf(str,"%x",value)==1)
      return 0;
   else
      return -1;
}
