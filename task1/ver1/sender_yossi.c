
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

#define SEND_TIME 500000000 //0.1 sec

char* stringToBinary(char* s)
{
    if(s == NULL) return 0; /* no input string */
    char * binary = malloc(16);
    char *ptr = s;
    int i;
	strcpy(binary,"");
    for(; *ptr != 0; ++ptr)
    {
        /* perform bitwise AND for every bit of the character */
        for(i = 7; i >= 0; --i){
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
	
	int nrecords = 50000;
	int slot = 10000;
	int noise_nrecords = 240000;
	int noise_slot = 20000;
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


