cp ..\dll-consume\dllsend2.dll .\dllsend2.dll
rm dllsend.dll
rm dllsend.obj
nasm -f win -o dllsend.obj .\main.asm
golink /dll /entry _start /fo dllsend.dll /export send .\dllsend.obj .\dllsend2.dll
rm ..\dll-consume\dllsend.dll
mv .\dllsend.dll ..\dll-consume\dllsend.dll