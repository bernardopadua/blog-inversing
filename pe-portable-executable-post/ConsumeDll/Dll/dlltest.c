#include <stdio.h>
#include <windows.h>

__declspec(dllexport) void NONAME (const char *nome) {
    printf("Olá, %s", nome);
}