/** @file */  // This line is to make doxygen happy.
/* **********************
This file should document the register map for the MAX2580 RF Prototype Board.
We will try to use Doxygen compatible comments wherever possible.
************************** */

/// This is the base address of the AXI I/O 
#define		BASE_ADDRESS		0x40000000

/// This is the size of the memory hole we need to talk to the FPGA.
#define		PROTO_SIZE		0x00100000

/// This is the offset of the register file that is used to control all the 
/// custom XG logic of the prototype. Individual registers should be described 
/// here as they are defined.
//#define 	AXI_REGFILE_0		0x00000000

/// This is a 32 bit register that provides an intentification word.
//#define 	FPGA_ID     		AXI_REGFILE_0+0x04*0  // Currently returns 0xDEADBEEF

/// Version register
//#define 	FPGA_VERSION		AXI_REGFILE_0+0x04*1  // Returns major and minor version numbers.

//#define 	LED_CONTROL     	AXI_REGFILE_0+0x04*7
///     bit 7:0     - KC705 User LED's. read/write.


#define     TEST_BRAM_OFFSET    0x00000000
#define     TEST_BRAM_SIZE      0x00002000 // 32K depth in bytes. divide by 4 to get depth in words.

/// Transaction Width for these SPI interfaces is set to 16 bits.
#define		TEST_SPI		    0x00010000
#define		DAC0_SPI		    0x00020000
#define		DAC1_SPI		    0x00030000

/// XADC interface for temperature and supply voltages.
//#define		XADC_IF 		    0x000D0000


