#include <stdio.h>
#include "dll.h"

char key = 0x5;

void encodeIt(PMSG pM){
	for(int i=0; pM->msg[i]; i++)	{
		pM->msg[i] = pM->msg[i] ^ key;
	}
}

void DLL_IE send(PMSG pM){
	char pressContinue;

	encodeIt(pM);
	printf("\n\tEnconded String> %s\n\tEncoded Message Sent> { ", pM->msg);
	for(int i=0; pM->msg[i]; i++)	{
		printf("0x%x ", pM->msg[i]);
	}

	printf("}\n\n> [enter] to continue;", pM->msg);
	scanf("%c", &pressContinue);
}