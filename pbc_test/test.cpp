#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pbc/pbc.h>

#include "src/sha256.h"
#include "src/aes.h"

using namespace std;

class Internal_data{
	public:
		uint32_t* get_DevAddr(){return DevAddr;}
		uint8_t* get_DevEUI(){return DevEUI;}
		uint8_t* get_NwkSKey(){return NwkSKey;}
	private:
		uint32_t DevAddr[4] = {0x26, 0x0D, 0xFA, 0x7F};
		uint8_t DevEUI[8] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x53, 0xC8};
		uint8_t NwkSKey[16] = {0x91,0x5A,0x14,0x90,0x72,0x7B,0x89,0xE6,0x5F,0x34,0x5D,0x4B,0xF1,0x43,0xE3,0xB6};
	
};

AES aes(AESKeyLength::AES_256);

void EN_registration();
void GW_registration();
void EN_Authentication();
void GW_Authentication();

int main(){
	Internal_data ID;
	EN_registration(); 
	//GW_registration();
	//EN_Authentication();
	//GW_Authentication();
	return 0;
}

void EN_registration(){
	Internal_data ID;
	int i,j,k;
	
	printf("End Node Registration\n");
	printf("===============================================================\n");
	
	srand(time(NULL));
	
	printf("Generating RN1\n");
	uint32_t RN1[4]={0,0,0,0};
					   
	for(i = 0; i < 4; i++) {RN1[i] = rand() % 256;printf("%d\n\n",RN1[i]);}
	
	printf("Generating M1\n");
	
	//M1:=(DevAddr||DevEUI||RN1)
	uint32_t devAddr[4]; *devAddr = *(ID.get_DevAddr)();
	uint8_t devEUI[8]; *devEUI = *(ID.get_DevEUI)();
	
	uint32_t M1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	
	for(i = 0; i < 4; i++) M1[i] = devAddr[i];
	uint8_t jj=0;
	for(j = 0; j < 8; j++) 
	{
		
		M1[j+4] = devEUI[jj];
		printf("%d", devEUI[jj]);
		jj++;
	}
	cout << "\n";
	for(k = 0; k < 4; k++) M1[k+12] = RN1[k];
	
	for(i=0;i<16;i++){
		printf("%d\n", M1[i]);
	}
	
	//strcat((char*) M1,(char*) devAddr);
	//printf("%s\n", (char*)M1);
	
	//strcat((char*) M1,(char*) devEUI);
	//printf("%s\n", (char*)M1);
	
	//strcat((char*) M1,(char*) RN1);
	//printf("%d\n", M1[1]);
	
	//printf("%ld\n",sizeof(M1));
	return;
}

void GW_registration(){
	return;
}
void EN_Authentication(){
	return;
}
void GW_Authentication(){
	return;
}

