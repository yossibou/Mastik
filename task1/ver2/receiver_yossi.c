
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <l3.h>
#include "timestats.h"
#include "low.h"
#include <unistd.h>
#include <inttypes.h>

#define SET 777
#define NRECORDS 8000
#define SLOT 12500
#define NOISE_NRECORDS 1000
#define NOISE_SLOT 20000
#define WORD_LENGTH 4

void binaryToString(char* input, char* output)
{
	char binary[9] = {0};
	for (int i = 0; i < 8; i ++)
	{
		binary[i] = input[i];    
	}
	*output  = strtol(binary,NULL,2); 
}

int zero_counter (l3pp_t l3, uint16_t *res,int nrecords)
{
	int i;
	int zero_count=0;
	for (i=0;i<nrecords;i++)
	{
		if (res[i]==0)
		{			
			zero_count++;
		}	
	}
	return zero_count;
}
l3pp_t find_set()
{
	int i;
	int zero_count=0;
	int nrecords = NOISE_NRECORDS;
	int slot = NOISE_SLOT;
	uint16_t *res = calloc(nrecords , sizeof(uint16_t));
	l3pp_t l3_1 = l3_prepare(NULL);
	for(i=0;i<4;i++)
	{
		l3_monitor(l3_1,SET+2048*i);
		l3_repeatedprobecount(l3_1, nrecords, res, slot);
		zero_count = zero_counter(l3_1,res,nrecords);
		if(zero_count<10)
		{
		free(res);
		return l3_1;
		}
		l3_unmonitorall(l3_1);
	}	
	free(res);
	return 0;
	
}
int main()
{
	//---------initialization---------
	int i,j;
	int zero_count=0;
	int nrecords = NRECORDS;
	int slot = SLOT;
	uint16_t *res = calloc(nrecords , sizeof(uint16_t));
	int noise_nrecords = NOISE_NRECORDS;
	int noise_slot = NOISE_SLOT;
	uint16_t *noise_res = calloc(noise_nrecords , sizeof(uint16_t));
	uint64_t prev_time;
	char* one = "1";
	char* zero = "0";
	char inputStr[32]= "";
	char outputStr[20]= {0};
	l3pp_t l3;
	while(1)
	{
		//---------Find set---------
		while(l3==0)
		{
		l3 = find_set();
		}
		//---------Wait until noise is stop---------
		while(zero_count<100){
			l3_repeatedprobecount(l3, noise_nrecords,noise_res,noise_slot);
			zero_count = zero_counter(l3,noise_res,noise_nrecords);
		}
		printf("ready\n");
		//---------Get the message---------
		for (j=0;j<WORD_LENGTH*8;j++)
		{
			zero_count = 0;
			prev_time = rdtscp64();
			while (rdtscp64()< prev_time+140000000);
			l3_repeatedprobecount(l3, nrecords, res, slot);
			zero_count = zero_counter(l3,res,nrecords);	
			if (zero_count<4000)
			{
				strcat(inputStr,one);
			}
			else
			{
				strcat(inputStr,zero);
			}					
		}
		//---------Print the message---------
	   	for (int i = 0; i < WORD_LENGTH; i++)
	   	{
			binaryToString(&inputStr[i*8], &outputStr[i]);
	   	}
	    	printf("%s\n", outputStr);
	    	l3=0;			
	}
}


