#ifndef _ZYNQ_GPIO_REGS_H_
#define _ZYNQ_GPIO_REGS_H_

#define GPIO_BASE_ADDR 0xE000A000 

#define MASK_DATA_0_LSW      0x00000000
#define MASK_DATA_0_MSW      0x00000004
#define MASK_DATA_1_LSW      0x00000008
#define MASK_DATA_1_MSW      0x0000000C
#define MASK_DATA_2_LSW      0x00000010
#define MASK_DATA_2_MSW      0x00000014
#define MASK_DATA_3_LSW      0x00000018
#define MASK_DATA_3_MSW      0x0000001C
#define DATA_0               0x00000040
#define DATA_1               0x00000044
#define DATA_2               0x00000048
#define DATA_3               0x0000004C
#define DATA_0_RO            0x00000060
#define DATA_1_RO            0x00000064
#define DATA_2_RO            0x00000068
#define DIRM_0               0x00000204
#define OEN_0                0x00000208
#define INT_MASK_0           0x0000020C
#define INT_EN_0             0x00000210
#define INT_DIS_0            0x00000214
#define INT_STAT_0           0x00000218
#define INT_TYPE_0           0x0000021C
#define INT_POLARITY_0       0x00000220
#define INT_ANY_0            0x00000224
#define DIRM_1               0x00000244
#define OEN_1                0x00000248
#define INT_MASK_1           0x0000024C
#define INT_EN_1             0x00000250
#define INT_DIS_1            0x00000254
#define INT_STAT_1           0x00000258
#define INT_TYPE_1           0x0000025C
#define INT_POLARITY_1       0x00000260
#define INT_ANY_1            0x00000264
#define DIRM_2               0x00000284
#define OEN_2                0x00000288 
#define INT_MASK_2           0x0000028C
#define INT_EN_2             0x00000290
#define INT_DIS_2            0x00000294
#define INT_STAT_2           0x00000298
#define INT_TYPE_2           0x0000029C
#define INT_POLARITY_2       0x000002A0
#define INT_ANY_2            0x000002A4
#define DIRM_3               0x000002C4
#define OEN_3                0x000002C8
#define INT_MASK_3           0x000002CC
#define INT_EN_3             0x000002D0
#define INT_DIS_3            0x000002D4
#define INT_STAT_3           0x000002D8
#define INT_TYPE_3           0x000002DC
#define INT_POLARITY_3       0x000002E0
#define INT_ANY_3            0x000002E4

#define REG_SIZE                     32
#define HALF_REG_SIZE                REG_SIZE/2
#define GPIO_PIN_IN_BANK(PIN)        (1<<(BIT%REG_SIZE))

//for 0<=PIN<=32
#define GPIO_BANK0_MIO_PIN(PIN)      (1<<PIN)
#define GPIO_BANK0_MIO_PIN_MASK(PIN) (~(1<<PIN))

//for 32<=PIN<=53
#define GPIO_BANK1_MIO_PIN(PIN)      (1<<(PIN-REG_SIZE))
#define GPIO_BANK1_MIO_PIN_MASK(PIN) (~(1<<(PIN-REG_SIZE)))


//for 0<=PIN<=15
#define SET_GPIO_MASK_DATA_0_LSW_MASK(PIN) ((~(1<<(PIN+HALF_REG_SIZE)))&0xFFFF0000)
//for 16<=PIN<=31
#define SET_GPIO_MASK_DATA_0_MSW_MASK(PIN) ((~(1<<(PIN-HALF_REG_SIZE+HALF_REG_SIZE)))&0xFFFF0000)
//for 32<=PIN<=47
#define SET_GPIO_MASK_DATA_1_LSW_MASK(PIN) ((~(1<<(PIN-32+HALF_REG_SIZE)))&0xFFFF0000)
//for 48<=PIN<=53
#define SET_GPIO_MASK_DATA_1_MSW_MASK(PIN) ((~(1<<(PIN-48+HALF_REG_SIZE)))&0xFFFF0000)

#define TO_STR(S)  #S

#endif//#define _ZYNQ_GPIO_REGS_H_
