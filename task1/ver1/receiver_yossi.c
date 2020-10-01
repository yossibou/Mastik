
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <l3.h>
#include "timestats.h"
#include "low.h"
#include <unistd.h>
#include <inttypes.h>

#define SET 123

void binaryToString(char* input, char* output){

    char binary[9] = {0}; // initialize string to 0's

    // copy 8 bits from input string
    for (int i = 0; i < 8; i ++){
        binary[i] = input[i];    
    }

    *output  = strtol(binary,NULL,2); // convert the byte to a long, using base 2 
}

int zero_counter (l3pp_t l3, uint16_t *res)
{
	int i;
	int zero_count=0;
	for (i=0;i<1000;i++)
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
	int nrecords = 1000;
	int slot = 20000;
	uint16_t *res = calloc(nrecords , sizeof(uint16_t));
	l3pp_t l3_1 = l3_prepare(NULL);

	for(i=0;i<4;i++)
	{
		l3_monitor(l3_1,SET+2048*i);
		l3_repeatedprobecount(l3_1, nrecords, res, slot);
		zero_count = zero_counter(l3_1,res);
		if(zero_count<10)
		{

		free(res);

		return l3_1;

		}
		l3_unmonitorall(l3_1);
	}	
	
	free(res);
	//l3_release(l3_1);
	return 0;
	
}
int main() {
while(1){
	int i,j;
	int zero_count=0,zero_count_temp;
	int nrecords = 10000;
	uint16_t *res = calloc(nrecords , sizeof(uint16_t));
	int slot = 15000;
	int noise_set=0;
	int noise_nrecords = 1000;
	int noise_slot = 20000;
	uint16_t *noise_res = calloc(noise_nrecords , sizeof(uint16_t));
	uint64_t prev_time;
	char* one;
	char* zero;
	one = "1";
	zero = "0";
	char inputStr[32]= "";
	char outputStr[20]= {0};
	size_t iterations;
	l3pp_t l3;

	while(l3==0){
	l3 = find_set()		;

	}

	while(zero_count<100){
		l3_repeatedprobecount(l3, noise_nrecords,noise_res,noise_slot);
		zero_count = zero_counter(l3,noise_res);

	}
	prev_time = rdtscp64();

	printf("ready\n");
	
	for (j=0;j<32;j++)
	{
		prev_time = rdtscp64();
		while (rdtscp64()< prev_time+175000000);
		zero_count =0;
		
		l3_repeatedprobecount(l3, nrecords, res, slot);	
		for (i=0;i<10000;i++)
		
		{
			if (res[i]==0)
			{
				zero_count++;
			}
		}
		
		if (zero_count<4000)
		{
			strcat(inputStr,one);

		}
		else
		{
			strcat(inputStr,zero);

		}
		
		prev_time = rdtscp64();
		while (rdtscp64()< prev_time+175000000);
		
	}

    	iterations = strlen(inputStr) / 8;
   	for (int i = 0; i < iterations; i++)
   	{
        	binaryToString(&inputStr[i*8], &outputStr[i]);
   	}
    	printf("%s\n", outputStr); // print the resulting string
    	l3=0;
		
}
}


