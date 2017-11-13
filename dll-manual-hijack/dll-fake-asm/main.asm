[BITS 32]

global _start

[section .drectve info align=8]
    db "kernel32.dll", 0x20
    db "msvcrt.dll", 0x20

section .idata
	extern __imp_send
    extern __imp_LoadLibraryA
    extern __imp_GetCurrentProcess
    extern __imp_VirtualProtectEx
    extern __imp_GetLastError
    extern __imp_puts
    extern __imp_fopen
    extern __imp_fclose
    extern __imp_fwrite
    extern __imp_strlen

section .data
    szdlltest   db "dllsend2.dll", 0
    addrdllsend dd 0
    expfunc     dd 0
    impfunc     dd 0
    nbfunc      db 0
    oldprt      dd 0
    
    filename    db "log.bin", 0
    fileopenmd  db "a+", 0
    logfile     dd 0

section .text

	_start:
    
        ;cleaning
        xor eax, eax
        xor ebx, ebx
    
        ;getting the module address
        push dword szdlltest
        call [__imp_LoadLibraryA]
        
        mov dword [addrdllsend], eax ;saving the dlltest2 base address
        
        ;grab exported functions of original DLL
        mov ebx, [eax+0x3c] ;e_lfanew NTHEADER-offset
        mov ebx, [eax+ebx+0x78] ;RVA export directory
        mov cl, [eax+ebx+0x14] ;number of functions
        mov [nbfunc], cl ;saving number of functions
        mov ebx, [eax+ebx+0x1c] ;addresses to array of functions
        add eax, ebx ;eax=baseaddress + ebx=RVA array of functions
        mov dword [expfunc], eax ;saving address of functions
        
        ;saving the IAT of fake dll
        mov eax, [esp+4] ;get dll base address on the stack
        mov ebx, [eax+0x3c] ;e_lfanew NTHEADER-offset
        mov ebx, [eax+ebx+0x80] ;RVA import directory
        mov ebx, [eax+ebx+0x10] ;getting the RVA to IAT from the first dll (dlltest)
        add eax, ebx ;Address for the IAT
        mov dword [impfunc], eax
        
        ;change mem protection
        xor ecx, ecx
        xor eax, eax
        xor edx, edx
        
        mov cx, [nbfunc] ;this block calculates the size to change mem protection in VirtualProtectEx.
        mov ax, 4 ;4=DWORD (pointer)
        mul cx
        mov ecx, eax
        
        xor eax, eax
        call [__imp_GetCurrentProcess]
        
        mov edx, [impfunc]
        
        push oldprt ;OUT PTR of VirtualProtect. pointer to receive previous access protection
        push 0x04 ;new protection READ_WRITE
        push ecx
        push edx ;mem address to change
        push eax
        call [__imp_VirtualProtectEx] ;looks for VirtualProtectEx in Microsoft MSDN
        
        cmp eax, 0 ;if 0 the protect failed
        je geterror
        
        ;cleaning
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx
        
        ;loop through the export and transfer to the imports
        mov bl, 0
        
        l_iexp:
        mov ax, 4 ;4=DWORD (pointer)
        mul bl 
        mov dx, ax ;index of array
        
        cmp bl, byte [nbfunc]
        je break_l

        mov eax, [expfunc] ;iterating through IAT
        mov eax, [eax+edx]
        add eax, dword [addrdllsend] ;exp address
        
        mov ecx, [impfunc]
        lea ecx, [ecx+edx]
        
        mov dword [ecx], eax
        
        add bl, 1
        jmp l_iexp

        geterror:
        call [__imp_GetLastError] ;Debugging purposes

        break_l:       
        mov eax, 1
        retn

	send:
        pushad ;save the register context in the stack

        push dword [eax]
        call [__imp_strlen]
        push eax ;keep in the stack

        push fileopenmd
        push filename
        call [__imp_fopen]
        mov dword [logfile], eax
        add esp, 8 ;cleaning the stack __cdecl

        pop eax ;strlen keeped in the stack
        pop ecx ;string parameter for original send
        push dword [logfile]
        push eax
        push 1
        push ecx
        call [__imp_fwrite]
        add esp, 16 ;cleaning the stack __cdecl

        push dword [logfile]
        call [__imp_fclose]
        add esp, 4 ;cleaning the stack __cdecl

        popad ;pop the register context back to each register

		jmp [__imp_send]
		retn