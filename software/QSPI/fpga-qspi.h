#ifndef _FPGA_QSPI_H_
#define _FPGA_QSPI_H_

//All information in this file comes from XILINX
//LogiCORE IP AXI Quad SPI v3.2 Product Guide Vivado Design Suite PG153 April 2, 2014

#define TRANSFER_WIDTH        16 //puting it in the command line parameter

#define TRANSFER_WIDTH_8_MASK                        0xFF
#define TRANSFER_WIDTH_16_MASK                       0xFFFF
#define TRANSFER_WIDTH_32_MASK                       0xFFFFFFFF
#define TRANSFER_WIDTH_MASK                          TRANSFER_WIDTH_16_MASK 

//Register Offsets
#define SRR                                          0x40
#define SPICR                                        0x60
#define SPISR                                        0x64
#define SPIDTR                                       0x68
#define SPIDRR                                       0x6C
#define SPISSR                                       0x70
#define SPI_TX_FIFO_OCY                              0x74
#define SPI_RX_FIFO_OCY                              0x78
#define DGIER                                        0x1C
#define IPISR                                        0x20
#define IPIER                                        0x28

//Register Field Masks
#define SSR_RESET                                    0x0000000a

#define SPICR_LSB_FIRST                              SET_BIT_X_TO_1(9)
#define SPICR_MASTER_TRANSACTION_INHIBIT             SET_BIT_X_TO_1(8)
#define SPICR_MANUAL_SLAVE_SELECT_ASSERTION_ENABLE   SET_BIT_X_TO_1(7)
#define SPICR_RX_FIFO_RESET                          SET_BIT_X_TO_1(6)
#define SPICR_TX_FIFO_RESET                          SET_BIT_X_TO_1(5)
#define SPICR_CPHA                                   SET_BIT_X_TO_1(4)
#define SPICR_CPOL                                   SET_BIT_X_TO_1(3)
#define SPICR_MASTER                                 SET_BIT_X_TO_1(2)
#define SPICR_SPE                                    SET_BIT_X_TO_1(1)
#define SPICR_LOOP                                   SET_BIT_X_TO_1(0)

#define SPISR_CMD_ERR                                SET_BIT_X_TO_1(10)
#define SPISR_LOOPBACK_ERR                           SET_BIT_X_TO_1(9)
#define SPISR_MSB_ERR                                SET_BIT_X_TO_1(8)
#define SPISR_SLAVE_MODE_ERR                         SET_BIT_X_TO_1(7)
#define SPISR_CPOL_PHHA_ERR                          SET_BIT_X_TO_1(6)
#define SPISR_SLAVE_MODE_SELECT                      SET_BIT_X_TO_1(5)
#define SPISR_MODF                                   SET_BIT_X_TO_1(4)
#define SPISR_TX_FULL                                SET_BIT_X_TO_1(3)
#define SPISR_TX_EMPTY                               SET_BIT_X_TO_1(2)
#define SPISR_RX_FULL                                SET_BIT_X_TO_1(1)
#define SPISR_RX_EMPTY                               SET_BIT_X_TO_1(0)

#define SPISSR_SS_0                                  BIT_X_TO_0(0)
#define SPISSR_SS_1                                  BIT_X_TO_0(1)
#define SPISSR_SS_2                                  BIT_X_TO_0(2)
#define SPISSR_SS_3                                  BIT_X_TO_0(3)

#define DGIER_GIE                                    SET_BIT_X_TO_1(31)

#define IPISR_CMD_ERR                                SET_BIT_X_TO_1(13)
#define IPISR_CMD_ERR                                SET_BIT_X_TO_1(13)
#define IPISR_LOOPBACK_ERR                           SET_BIT_X_TO_1(12)
#define IPISR_MSB_ERR                                SET_BIT_X_TO_1(11)
#define IPISR_SLAVE_MODE_ERR                         SET_BIT_X_TO_1(10)
#define IPISR_CPOL_CPHA_ERR                          SET_BIT_X_TO_1(9)
#define IPISR_DRR_NOT_EMPTY                          SET_BIT_X_TO_1(8)
#define IPISR_SLAVE_SELECT_MODE                      SET_BIT_X_TO_1(7)
#define IPISR_TX_FIFO_HALF_EMPTY                     SET_BIT_X_TO_1(6)
#define IPISR_DRR_OVERRUN                            SET_BIT_X_TO_1(5)
#define IPISR_DRR_FULL                               SET_BIT_X_TO_1(4)
#define IPISR_DTR_UNDERRUN                           SET_BIT_X_TO_1(3)
#define IPISR_DTR_EMPTY                              SET_BIT_X_TO_1(2)
#define IPISR_SLAVE_MODF                             SET_BIT_X_TO_1(1)
#define IPISR_MODF                                   SET_BIT_X_TO_1(0)

//routing the SPI interrupt signal to IRQF2P[18/19] for CPU0/1 to generate interrupt 28
//routing the SPI interrupt signal to IRQF2P[16/17] for CPU0/1 to generate interrupt 31
//from table 7-2 on page 217 on Zynq-7000 AP Soc Technical Reference Manual.
#define IPIER_CMD_ERR                                SET_BIT_X_TO_1(13)
#define IPIER_CMD_ERR                                SET_BIT_X_TO_1(13)
#define IPIER_LOOPBACK_ERR                           SET_BIT_X_TO_1(12)
#define IPIER_MSB_ERR                                SET_BIT_X_TO_1(11)
#define IPIER_SLAVE_MODE_ERR                         SET_BIT_X_TO_1(10)
#define IPIER_CPOL_CPHA_ERR                          SET_BIT_X_TO_1(9)
#define IPIER_DRR_NOT_EMPTY                          SET_BIT_X_TO_1(8)
#define IPIER_SLAVE_SELECT_MODE                      SET_BIT_X_TO_1(7)
#define IPIER_TX_FIFO_HALF_EMPTY                     SET_BIT_X_TO_1(6)
#define IPIER_DRR_OVERRUN                            SET_BIT_X_TO_1(5)
#define IPIER_DRR_FULL                               SET_BIT_X_TO_1(4)
#define IPIER_DTR_UNDERRUN                           SET_BIT_X_TO_1(3)
#define IPIER_DTR_EMPTY                              SET_BIT_X_TO_1(2)
#define IPIER_SLAVE_MODF                             SET_BIT_X_TO_1(1)
#define IPIER_MODF                                   SET_BIT_X_TO_1(0)

#endif//_FPGA_QSPI_H_
