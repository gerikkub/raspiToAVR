#ifndef __ISPATTINY_H__
#define __ISPATTINY_H__

#define ISP_CMD_PROG_ENABLE_1	0xAC
#define ISP_CMD_PROG_ENABLE_2	0x53

#define ISP_CMD_CHIP_ERASE_1	0xAC
#define ISP_CMD_CHIP_ERASE_2	0x80

#define ISP_CMD_READ_MEM_1	0x20

#define ISP_CMD_WRITE_MEM_1	0x40

#define ISP_CMD_READ_EEPROM_1	0xA0

#define ISP_CMD_WRITE_EEPROM_1	0xC0

#define ISP_CMD_WRITE_LOCK_1	0xA3
#define ISP_CMD_WRITE_LOCK_2	0xF9

#define ISP_CMD_READ_LOCK_1	0x58

#define ISP_CMD_READ_SIG_1		0x30

#define ISP_CMD_READ_CAL_1		0x38

#define ISP_CMD_WRITE_FUSE_1	0xAC
#define ISP_CMD_WRITE_FUSE_2	0xA0

#define ISP_CMD_READ_FUSE_1	0x50


#define HIGH_BYTE	1
#define LOW_BYTE	0

#define LOCK_ON	1
#define LOCK_OFF	0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

void initializeSPI();
uint8_t programmingEnable();
void chipErase();
uint16_t readProgramMemory(int addr);
uint8_t readProgramMemoryByte(int addr,int which);
void writeProgramMemoryByte(int addr,uint8_t byte,int which);
void writeProgramMemory(int addr,uint16_t data);
void writeLockBits(int lock0,int lock1);
uint8_t readLockBits();
uint8_t readSignatureByte(int byte);
uint8_t readCalibrationByte();
void writeFuseBits(int bits);
uint8_t ATtiny_readFuseBits();

#endif
