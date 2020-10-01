
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <l3.h>

#include "timestats.h"
#include "low.h"
#include <unistd.h>
#include <inttypes.h>
#include <assert.h>


#define SET 777
#define NRECORDS 240000
#define SLOT 10000
#define NOISE_NRECORDS 240000
#define NOISE_SLOT 20000


#define SEND_TIME 2400000000 //0.1 sec



char* stringToBinary(char* s)
{
	if(s == NULL) return 0;
	char * binary = malloc(16);
	char *ptr = s;
	int i;
	strcpy(binary,"");
	for(; *ptr != 0; ++ptr)
	{
		for(i = 7; i >= 0; --i)
		{
			(*ptr & 1 << i) ? strcat(binary,"1") : strcat(binary,"0");
		}
	}
	return binary;
}


int main() 
{
	//---------initialization---------
	uint64_t i,j,k;
	uint64_t t1=0,t2=0;
	char* message;
	char* binarymsg;
	void* line;
	l3pp_t l3 = l3_prepare(NULL);
	//---------send message-----------
	printf("Write your message\n");
	scanf("%s", message);
	binarymsg = stringToBinary(message);
	printf("%s\n", binarymsg);
	//---------make a noise-----------
	for(i=0;i<20000000;i++)
	{
		for(j=0;j<11;j++)
		{
			memaccess(l3_getline(l3,SET,j));
		}
	}
	printf("Noise finish\n");
	for(i=0;i<8;i++)
	{	
		
		if(binarymsg[i]=='1')
		{
			for(k=0;k<11000000;k++)
			{
				for(j=0;j<11;j++)
				{
					memaccess(l3_getline(l3,SET,j));
				}
			}		
		}
		else
		{
			for (j=0;j<SEND_TIME/4.7;j++);
		}	
	}
}


