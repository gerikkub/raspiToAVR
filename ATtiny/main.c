#include <stdio.h>
#include <assert.h>

#include "isp.h"

int hexCharToInt(char a){
	switch(a){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return a - 0x30;
	case 'a':
	case 'A':
		return 0xA;
	case 'b':
	case 'B':
		return 0xB;
	case 'c':
	case 'C':
		return 0xC;
	case 'd':
	case 'D':
		return 0xD;
	case 'e':
	case 'E':
		return 0xE;
	case 'f':
	case 'F':
		return 0xF;
	default:
		printf("Invalid character %c\n",a);
		return -1;
	}
}


int main(int argc,char** argv){

	FILE* inputHex;
	int fileSize;
	int byteCount;
	int addr;
	int highLowToggle = LOW_BYTE;
	int ndx;
	uint16_t data;
	char hexData[128];

	inputHex = fopen(argv[1],"r");

	initializeSPI();

	if(programmingEnable() != 0x53){
		printf("Programmer Out of Sync!!!\n");
	} else {
		printf("In sync!\n");
	}

	//printf("Erasing chip\n");
	if(argv[2][0] == 'e'){
		chipErase();
		return 1;
	}

	//writeProgramMemoryByte(0,0xFF,LOW_BYTE);

	//chipErase();

	usleep(25000);

	//printf("Fuse check: %hhX\n",readFuseBits());
	
	//return 1;

	while(feof(inputHex) == 0){
		fgets(hexData,128,inputHex);
		printf("parsing %s\n",hexData);
		if(hexData[0] != ':'){
			printf("Error in hex file!\n");
		}
		byteCount = hexCharToInt(hexData[1]) << 4;
		byteCount |= hexCharToInt(hexData[2]);
	
		addr = hexCharToInt(hexData[3]) << 12;
		addr |= hexCharToInt(hexData[4]) << 8;
		addr |= hexCharToInt(hexData[5]) << 4;
		addr |= hexCharToInt(hexData[6]);

		addr /= 2;

		if(hexData[8] == '1'){
			break;
		} else if(hexData[8] != '0'){
			printf("Unsupported Hex file\n");
			break;
		}

		ndx = 9;
		while(byteCount){
			data = hexCharToInt(hexData[ndx]) << 4;
			ndx++;
			data |= hexCharToInt(hexData[ndx]);
			ndx++;
			data |= hexCharToInt(hexData[ndx]) << 12;
			ndx++;
			data |= hexCharToInt(hexData[ndx]) << 8;
		
			printf("Writing byte %hX to address %hX\n",data,addr);

			writeProgramMemory(addr,data);
			usleep(4000);
			ndx++;
			addr++;
			byteCount -= 2;
		}
	}

	byteCount = 0;
	while(byteCount <= 11){
		printf("%X: %hX\n",byteCount,readProgramMemory(byteCount));

		//printf("%X: %hhX %hhX\n",byteCount,readProgramMemoryByte(byteCount,HIGH_BYTE),readProgramMemoryByte(byteCount,LOW_BYTE));
		byteCount++;
	}

	fclose(inputHex);
}	
			
		
		
	



