#ifndef __UTILS_H__
#define __UTILS_H__

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG(fmt, args...) fprintf(stdout,"@%s:%d, " fmt,__FUNCTION__,__LINE__,## args)
#else //!DEBUG_MODE
#define DEBUG(fmt, args...)
#endif //DEBUG_MODE

#define ERR_LOG(fmt, args...) fprintf(stderr,"@%s:%d, " fmt,__FUNCTION__,__LINE__,## args)

void print_hex_info(const char* title,int calling_line_num,const unsigned char* hex_info,unsigned int info_size); 
int str_to_hex32_bits(const char* str,uint32_t* value);

#endif //__UTILS_H__
