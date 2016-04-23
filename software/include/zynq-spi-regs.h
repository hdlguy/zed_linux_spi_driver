#ifndef _ZYNQ_SPI_REGS_H_
#define _ZYNQ_SPI_REGS_H_

#define SPI_0_BASE_ADDR          0xE0006000
#define SPI_1_BASE_ADDR          0xE0007000

#define CONFIG_REG               0x00000000
#define INTR_STATUS_REG          0x00000004
#define INTRPT_EN_REG            0x00000008
#define INTRPT_DIS_REG           0x0000000C
#define INTRPT_MASK_REG          0x00000010
#define EN_REG                   0x00000014
#define DELAY_REG                0x00000018
#define TX_DATA_REG              0x0000001C
#define RX_DATA_REG              0x00000020
#define SLAVE_IDLE_COUNT_REG     0x00000024
#define TX_THRES_REG             0x00000028
#define RX_THRES_REG             0x0000002C
#define MOD_ID_REG               0x000000FC

#endif// _ZYNQ_SPI_REGS_H_
