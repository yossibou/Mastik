
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <l3.h>
#include "timestats.h"
#include "low.h"
#include <unistd.h>
#include <inttypes.h>

#define SET 777
#define NRECORDS 24000
#define SLOT 10000
#define NOISE_NRECORDS 240000
#define NOISE_SLOT 20000

#define SEND_TIME 240000000 //0.1 sec

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

int main() {
	//---------initialization---------
	int i;
	char* message;
	char* binarymsg;
	int nrecords = NRECORDS;
	int slot = SLOT;
	int noise_nrecords = NOISE_NRECORDS;
	int noise_slot = NOISE_SLOT;
	uint16_t *noise_results = calloc(noise_nrecords, sizeof(uint16_t));
	uint16_t *results = calloc(nrecords, sizeof(uint16_t));
	uint64_t prev_time = 0;
	l3pp_t l3 = l3_prepare(NULL);
	//---------send message-----------
	printf("Write your message\n");
	scanf("%s", message);
	binarymsg = stringToBinary(message);
	printf("%s\n", binarymsg);
	//---------make a noise-----------
	l3_monitor(l3,SET);
	l3_repeatedprobecount(l3, noise_nrecords, noise_results, noise_slot);
	printf("Noise finish\n");
	
	for(i=0;i<32;i++)
	{	
		
		if(binarymsg[i]=='1')
		{	
		
			l3_repeatedprobecount(l3, nrecords, results, slot);
				
		}
		else
		{
			prev_time = rdtscp64();
			while (rdtscp64()< prev_time+SEND_TIME);
		}	
	}	
}


