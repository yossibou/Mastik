
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
#define NRECORDS 80000
#define SLOT 12500
#define NOISE_NRECORDS 1000
#define NOISE_SLOT 20000
#define WORD_LENGTH 1
int set;

void binaryToString(char* input, char* output)
{
	char binary[9] = {0};
	for (int i = 0; i < 8; i ++)
	{
		binary[i] = input[i];    
	}
	*output  = strtol(binary,NULL,2); 
}
l3pp_t find_set()
{
	int i,m,k=0;
	uint64_t prev_time;
	fflush(stdout);
	l3pp_t l3_1 = l3_prepare(NULL);
	for(i=0;i<4;i++)
	{
		prev_time = rdtscp64();
		while (rdtscp64()< prev_time+2400000)
		{
			for (m=0;m<11;m++)
				{
					//l3_getline(l3_1,SET + 2048*i,m);
					memaccess(l3_getline(l3_1,SET + 2048*i,m));
				}
				k++;
				//fflush(stdout);
		}
		if (k<3000)
		{
			set = SET + 2048*i;
			return l3_1;
		}
		k=0;	
	}	
	return 0;
}
int main()
{
	//---------initialization---------
	int i,j,m;
	int k=0;
	uint64_t prev_time;
	char* one = "1";
	char* zero = "0";
	char inputStr[32]= "";
	char outputStr[20]= {0};
	l3pp_t l3;
	fflush(stdout);
	//---------Find set---------
	while(l3==0)
	{
	fflush(stdout);
	l3 = find_set();
	printf("ready\n");	
	}
	//---------Wait until noise is stop---------
	while(k<5000){
	printf("go\n");
		k=0;
		prev_time = rdtscp64();
		while (rdtscp64()< prev_time+2400000)
		{
			for (m=0;m<11;m++)
				{
					memaccess(l3_getline(l3,set,m));
				}
				k++;
				fflush(stdout);
		}
	}
	printf("set\n");
	//---------Get the message---------
	for (j=0;j<WORD_LENGTH*8;j++)
	{
		for (i=0;i<1400000000/4.7;i++);
		prev_time = rdtscp64();
		while (rdtscp64()< prev_time+1000000000)
			{
				for (m=0;m<11;m++)
				{
					memaccess(l3_getline(l3,set,m));
				}
				k++;	
			}
		printf("%d\n",k);
		if (k<1500000)
		{
			strcat(inputStr,one);
		}
		else
		{
			
			strcat(inputStr,zero);
		}
		k=0;					
	}
	
	//---------Print the message---------
   	for (int i = 0; i < WORD_LENGTH; i++)
   	{
		binaryToString(&inputStr[i*8], &outputStr[i]);
   	}
    	printf("%s\n", outputStr);
    	l3=0;			

	
}


