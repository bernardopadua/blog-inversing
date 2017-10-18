#include <stdio.h>
#include <windows.h>

__declspec(dllexport) void NONAME (const char *name) {
    printf("Hello, %s", name);
}
