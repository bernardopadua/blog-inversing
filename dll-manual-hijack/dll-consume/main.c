#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\dll\dll.h"

#define MAX_MSG 1024

void header(){
	system("cls");
	puts("################################");
	puts("# Blog VerseInversing          #");
	puts("# Manual Dll Hijacking         #");
	puts("# Author: Pimptech             #");
	puts("# Only for learning purposes!  #");
	puts("################################");
}

void initMessage(PMSG pMessage){
	pMessage->msg = malloc(MAX_MSG);
}

void grabMessage(PMSG pM){
	int tamStr = 0;
	printf("\tType your message: ");
	//scanf("%s", pM->msg);
	fgets(pM->msg, MAX_MSG, stdin);
	tamStr = strlen(pM->msg);
	((char*)pM->msg)[tamStr-1] = '\0';
}

int main(int agrv, char* argc[]){
	MSG message;
	initMessage(&message);

	while(1==1){
		header();
		grabMessage((PMSG)&message);
		send((PMSG)&message);
	}

	return 0;
}