#ifndef __ISPATTINY_H__
#define __ISPATTINY_H__

#define ISP_CMD_PROG_ENABLE_1	0xAC
#define ISP_CMD_PROG_ENABLE_2	0x53

#define ISP_CMD_CHIP_ERASE_1	0xAC
#define ISP_CMD_CHIP_ERASE_2	0x80

#define ISP_CMD_READ_MEM_1	0x20

#define ISP_CMD_LOAD_EXTEND_ADDR_1	0x4D

#define ISP_CMD_LOAD_MEM_PAGE_HIGH_1	0x48

#define ISP_CMD_LOAD_MEM_PAGE_LOW_1		0x40

#define ISP_CMD_WRITE_MEM_PAGE_1	0x4C

#define ISP_CMD_READ_EEPROM_1	0xA0

#define ISP_CMD_WRITE_EEPROM_1	0xC0

#define ISP_CMD_WRITE_LOCK_1	0xA3
#define ISP_CMD_WRITE_LOCK_2	0xF9

#define ISP_CMD_READ_LOCK_1	0x58

#define ISP_CMD_READ_SIG_1		0x30

#define ISP_CMD_READ_CAL_1		0x38

#define ISP_CMD_WRITE_FUSE_1	0xAC
#define ISP_CMD_WRITE_FUSE_2	0xA0

#define ISP_CMD_WRITE_FUSE_HIGH_1	0xAC
#define ISP_CMD_WRITE_FUSE_HIGH_2	0xA8

#define ISP_CMD_WRITE_EXTEND_FUSE_1	0xAC
#define ISP_CMD_WRITE_EXTEND_FUSE_2	0xA4

#define ISP_CMD_READ_FUSE_1	0x50

#define ISP_CMD_READ_FUSE_HIGH_1	0x58
#define ISP_CMD_READ_FUSE_HIGH_2	0x08

#define ISP_CMD_READ_EXTEND_FUSE_1	0x50
#define ISP_CMD_READ_EXTEND_FUSE_2	0x08

#define ISP_CMD_READ_MEM_HIGH_1	0x28

#define ISP_CMD_READ_MEM_LOW_1	0x20

#define ATMEGA_328P 	1
#define ATMEGA_168PA	2
#define ATMEGA_88PA	3
#define ATMEGA_48PA	4

#define HIGH_BYTE	1
#define LOW_BYTE	0

#define LOCK_ON	1
#define LOCK_OFF	0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

void setATmega(int a);
void initializeSPI();
uint8_t programmingEnable();
void chipErase();
uint16_t readProgramMemory(int addr);
uint8_t readProgramMemoryByte(int addr,int which);
void loadExtendedAddress(int addr);
void writeProgramMemoryByte(int addr,uint8_t byte,int which);
void writeProgramMemory(int addr,uint16_t data);
void writeProgramMemoryPage(int addr);
void finishProgramming();
void writeLockBits(int lock0,int lock1);
uint8_t readLockBits();
uint8_t readSignatureByte(int byte);
uint8_t readCalibrationByte();
void writeFuseBits(int bits);
void writeFuseHighBits(int bits);
void writeExtendedFuseBits(int bits);
uint8_t readFuseBits();
uint8_t readFuseHighBits();
uint8_t readExtendedFuseBits();

#endif
