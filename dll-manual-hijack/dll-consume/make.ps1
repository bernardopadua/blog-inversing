rm m.exe
rm main.o
rm log.bin
gcc -c -DLL_IMPORT .\main.c
gcc -o m.exe .\main.o -L.\ -ldllsend2