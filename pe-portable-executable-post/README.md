#PE - Portable Executable File

Hello, Guys!

It's better using **PowerShell**. I promise the next post I will let a MAKEFILE to keep objective and easy.

##Dll Consume

Entering the the folder.
> cd .\ConsumeDll

Let's compile first the **dlltest.dll** and to do this:
> cd Dll
> gcc -c dlltest.c
> gcc -shared .\dlltest.o -o dlltest.dll
> mv .\dlltest.dll ..\

Now we have to compile the **loadlib_test.exe**:
> cd ..\Loadlib_dll
> gcc .\loadlib_test.c -o loadlib_test.exe ..\dlltest.dll
> mv loadlib_test.exe ..\

You can execute it now.
> cd ..
> .\loadlib_test.exe

##Resource Files

Entering the the folder.
> cd .\ResourceFile

Let's compile the source inside of the folder **inversing** to get our resource file. To do this you can use the following command:
> cd inversing
> gcc -o inversing.exe inversing.c
> mv .\inversing.exe ..\
> cd ..

We need to compile the **res.rc** to an *object file*, to be compiled with the main source **blog_resource.c**:
> windres res.rc res.o

Now we have generated the *object file* **res.o** and we can compile the main source:
> gcc res.o blog_resource.c -o blog_resource.exe
> .\blog_resource.exe

It's done. Very easy! :)