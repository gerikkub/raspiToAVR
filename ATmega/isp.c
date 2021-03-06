#include <wiringPi.h>

#include "isp.h"

const int resetPin = 8;
const int mosi = 10;
const int miso = 9;
const int sck = 11;

const int clockSpeed = 10;

static int WordBits;
static int WordMask;

static int lastPageAddr = -1;

typedef unsigned char uint8_t;

uint8_t bitBangSPIByte(uint8_t byte){

	int bitMask;
	unsigned char input = 0;
	
	for(bitMask = 0x80;bitMask;bitMask >>= 1){
		if(bitMask & byte){
			digitalWrite(mosi,HIGH);
		} else {
			digitalWrite(mosi,LOW);
		}

		usleep(clockSpeed);

		digitalWrite(sck,HIGH);

		if(digitalRead(miso)) {
			input |= bitMask;
		}

		usleep(clockSpeed);
		digitalWrite(sck,LOW);
	}
	return input;
}

void bitBangSPIWord(uint8_t* data){
	data[0] = bitBangSPIByte(data[0]);
	data[1] = bitBangSPIByte(data[1]);
	data[2] = bitBangSPIByte(data[2]);	
	data[3] = bitBangSPIByte(data[3]);
}

void setATmega(int a){
	switch(a){
	case ATMEGA_328P:
		WordBits = 6;
		WordMask = 0x3F;
		break;
	case ATMEGA_168PA:
		WordBits = 6;
		WordMask = 0x3F;
		break;
	case ATMEGA_88PA:
		WordBits = 5;
		WordMask = 0x1F;
		break;
	case ATMEGA_48PA:
		WordBits = 5;
		WordMask = 0x1F;
		break;
	}
}

void initializeSPI(){
	wiringPiSetupGpio();

	pinMode(resetPin,OUTPUT);
	pinMode(mosi,OUTPUT);
	pinMode(sck,OUTPUT);
	pinMode(miso,INPUT);

	digitalWrite(sck,LOW);

	usleep(100);

	digitalWrite(resetPin,HIGH);

	usleep(50);

	digitalWrite(resetPin,LOW);

	usleep(25000);
}

uint8_t programmingEnable(){
	uint8_t cmd[] = {ISP_CMD_PROG_ENABLE_1,ISP_CMD_PROG_ENABLE_2,0,0};
	
	bitBangSPIWord(cmd);
	return cmd[2];
}

void chipErase(){
	uint8_t cmd[] = {ISP_CMD_CHIP_ERASE_1,ISP_CMD_CHIP_ERASE_2,
							0,0};
	bitBangSPIWord(cmd);

	usleep(7000);

	digitalWrite(resetPin,HIGH);
	
	usleep(10);
	
	digitalWrite(resetPin,LOW);

	usleep(25000);
}

void loadExtendedAddress(int addr){
	uint8_t cmd[] = {ISP_CMD_LOAD_EXTEND_ADDR_1,0,0,0};
	cmd[2] = addr >> WordBits;
	printf("Loading Address %hhX\n",cmd[2]);
	bitBangSPIWord(cmd);
}	

uint8_t readProgramMemoryByte(int addr,int which){
	uint8_t cmd[] = {0,0,0,0};
	if(which == HIGH_BYTE){
		cmd[0] = ISP_CMD_READ_MEM_HIGH_1;
	} else {
		cmd[0] = ISP_CMD_READ_MEM_LOW_1;
	}
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0xFF;
	bitBangSPIWord(cmd);
	return cmd[3];
}

uint16_t readProgramMemory(int addr){
	return readProgramMemoryByte(addr,LOW_BYTE) | (readProgramMemoryByte(addr,HIGH_BYTE) << 8);
}

void writeProgramMemoryByte(int addr,uint8_t byte,int which){
	uint8_t cmd[] = {0,0,0,0};

	if(lastPageAddr != (addr >> WordBits)){
		writeProgramMemoryPage(lastPageAddr << WordBits);
		usleep(4500);
		loadExtendedAddress(addr);
		lastPageAddr = (addr >> WordBits);
	}

	if(which == HIGH_BYTE){
		cmd[0] = ISP_CMD_LOAD_MEM_PAGE_HIGH_1;
		//printf("High %hhX\n",byte);
	} else {
		cmd[0] = ISP_CMD_LOAD_MEM_PAGE_LOW_1;
		//printf("Low %hhX\n",byte);
	}

	cmd[2] = addr & WordMask;
	cmd[3] = byte;

	bitBangSPIWord(cmd);
}

void writeProgramMemory(int addr,uint16_t data){
	writeProgramMemoryByte(addr,data & 0xFF,LOW_BYTE);
	usleep(5500);
	writeProgramMemoryByte(addr,data >> 8,HIGH_BYTE);
	usleep(5500);
}

void writeProgramMemoryPage(int addr){
	uint8_t cmd[] = {ISP_CMD_WRITE_MEM_PAGE_1,0,0,0};
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0xFF;
	printf("Writing page with addr %hhX%hhX\n",cmd[1],cmd[2]);
	bitBangSPIWord(cmd);
}

void finishProgramming(){
	writeProgramMemoryPage(lastPageAddr << WordBits);
}

void writeLockBits(int lock0,int lock1){
	uint8_t cmd[] = {ISP_CMD_WRITE_LOCK_1,ISP_CMD_WRITE_LOCK_2,0,0};

	if(lock0 == LOCK_ON){
		cmd[1] |= 0x02;
	}

	if(lock1 == LOCK_ON){
		cmd[1] |= 0x04;
	}
	
	bitBangSPIWord(cmd);
}

uint8_t readLockBits(){
	uint8_t cmd[] = {ISP_CMD_READ_LOCK_1,0,0,0};
	bitBangSPIWord(cmd);
	return cmd[0] >> 1;
}

uint8_t readSignatureByte(int byte){
	uint8_t cmd[] = {ISP_CMD_READ_SIG_1,0,0,0};
	cmd[2] = byte;
	bitBangSPIWord(cmd);
	return cmd[3];
}

uint8_t readCalibrationByte(){
	uint8_t cmd[] = {ISP_CMD_READ_CAL_1,0,0,0};
	bitBangSPIWord(cmd);
	return cmd[3];
}

void writeFuseBits(int bits){
	uint8_t cmd[] = {ISP_CMD_WRITE_FUSE_1,ISP_CMD_WRITE_FUSE_2,0,0};
	cmd[3] = bits;
	bitBangSPIWord(cmd);
}

void writeFuseHighBits(int bits){
	uint8_t cmd[] = {ISP_CMD_WRITE_FUSE_HIGH_1,ISP_CMD_WRITE_FUSE_HIGH_2,0,0};
	cmd[3] = bits;
	bitBangSPIWord(cmd);
}

void writeExtendFuseBits(int bits){
	uint8_t cmd[] = {ISP_CMD_WRITE_EXTEND_FUSE_1,ISP_CMD_WRITE_EXTEND_FUSE_2,0,0};
	cmd[3] = bits;
	bitBangSPIWord(cmd);
}

uint8_t readFuseBits(){
	uint8_t cmd[] = {ISP_CMD_READ_FUSE_1,0,0,0};
	bitBangSPIWord(cmd);
	return cmd[3];
}

uint8_t readFuseHighBits(){
	uint8_t cmd[] = {ISP_CMD_READ_FUSE_HIGH_1,ISP_CMD_READ_FUSE_HIGH_2,0,0};
	bitBangSPIWord(cmd);
	return cmd[3];
}

uint8_t readExtendedFuseBits(){
	uint8_t cmd[] = {ISP_CMD_READ_EXTEND_FUSE_1,ISP_CMD_READ_EXTEND_FUSE_2,0,0};
	bitBangSPIWord(cmd);
	return cmd[3];
}
