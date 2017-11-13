rm .\dll_c.o
gcc -c .\dll_c.c
gcc -shared -o dllsend.dll .\dll_c.o
rm ..\dll-consume\dllsend2.dll
mv .\dllsend.dll ..\dll-consume\dllsend2.dll