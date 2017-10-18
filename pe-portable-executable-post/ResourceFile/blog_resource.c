#include <stdio.h>
#include <windows.h>

int main(void){
    
    puts("\nHello, Inversing! Did you knew that I have an executable inside me ?");
    puts("The power of resource files!");
    
    HRSRC pResource = FindResource(NULL, MAKEINTRESOURCE(2), "EXE");
    HGLOBAL pExe = LoadResource(NULL, pResource);
    
    char resFile[3] = {0x0, 0x0, 0x0};
    memcpy(resFile, pExe, 2);
    
    printf("\n\n\tResource at: 0x%0.8x\n", pExe);
    printf("\tFirst bytes: %s\n", resFile);
    puts("\tAn executable!\n");
    
    return 0;
}