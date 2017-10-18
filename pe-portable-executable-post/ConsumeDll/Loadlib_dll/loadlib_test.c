#include <stdio.h>
#include <windows.h>

int main(void){

    HMODULE dll;
    typedef void (WINAPI *NONAME)(const char *text);

    dll = LoadLibraryA("dlltest.dll");
    if(dll==NULL){
        puts("Error Loading DLL!");
        return 0;
    }

    NONAME myFunc = (NONAME)GetProcAddress(dll, "NONAME");

    printf("ADDR: %8x", myFunc);

    return 0;

}