#include <stdio.h>
#include <windows.h>
#include <strings.h>

void px(DWORD t, const char* d){
    printf("\t%s> [ 0x%x ]\n", d, t);
}

void convertEndianess(size_t psize, void* value){
    char r;
    for(int i=0;i<(psize/2);i++){ //To change in one time-loop DWORD=4. (4/2).
        r                         = ((char*)value)[i];
        ((char*)value)[i]         = ((char*)value)[psize-i-1];
        ((char*)value)[psize-i-1] = r;
    }
}

int main(int args, char *vargs[]){
    
    FILE *peFile;
    char *buf;
    long fSize;
    int totalRead = 0;
    
    PIMAGE_NT_HEADERS     ntHeader;
    PIMAGE_SECTION_HEADER secHeader;
    PIMAGE_EXPORT_DIRECTORY exportDirectory;
    
    if(args<=1){
        puts("Please inform the file path;");
        return 0;
    }
    puts("############################");
    puts("  DLL Exporter              ");
    puts("  Author: Pimptech          ");
    puts("  Blog VerseInversing       ");
    puts("  https://verseinversing.blogspot.com");
    puts("############################\n");

    peFile = fopen(vargs[1], "r");
    
    //Getting file size;
    fseek(peFile, 0, SEEK_END);
    fSize = ftell(peFile);
    rewind(peFile);
    //End file size;
    
    //Allocating space for file buffer;
    buf = calloc(fSize+1, 1);
    px((DWORD)buf, "DLL BaseAddress");

    //Moving bytes to the buffer (reading) file;
    totalRead = fread(buf, sizeof(char), fSize, peFile);
    
    if(totalRead > 0){        
        DWORD *pt = (DWORD*)&buf[0x3c]; //Getting e_lfanew (PE Header)
        
        //Getting e_lfanew (PE Header) - For some reason it did movsx forcing the value to 0xFFFFFF80 ('*-1' fix this)
        //Happened even with 'unsigned int'
        DWORD e_lfanew = ((DWORD)buf[0x3c])*-1;

        DWORD exportRVA; //RVA from expor directory
        DWORD exportOffset; //Offset of export section;
        DWORD *addrFuncs; //Offset of names;
        DWORD nOfFunc; //Number of functions;
        
        FILE *fExt, *fSubs;
        
        char dllExt[50]={0};
        char dllSubs[50]={0};
        
        ntHeader  = (PIMAGE_NT_HEADERS)&buf[e_lfanew-1]; //Get address of the begining of PE header;
        secHeader = (PIMAGE_SECTION_HEADER)&buf[e_lfanew+sizeof(IMAGE_NT_HEADERS)-1]; //Getting sections;
        exportRVA = ntHeader->OptionalHeader.DataDirectory[0].VirtualAddress;

        px((DWORD)ntHeader, "Ntheader");
        px((DWORD)secHeader, "Section Header");
        px(exportRVA, "RVA Export Section");

        int n = 0; //Loop through sections

        while(secHeader[n].VirtualAddress != '\0'){
            if(secHeader[n].VirtualAddress == exportRVA){
                exportOffset = secHeader[n].PointerToRawData; //Offset of export sections;
                convertEndianess(sizeof(DWORD), &exportOffset);
                px(exportOffset, "exportOffset");
                break;
            }
            n++;
        }
        
        //When information is loaded in register endianess changes
        convertEndianess(sizeof(DWORD), &exportRVA);

        exportDirectory = (PIMAGE_EXPORT_DIRECTORY)&buf[exportOffset];
        nOfFunc         = exportDirectory->NumberOfFunctions;
        addrFuncs       = (DWORD*)&buf[(exportDirectory->AddressOfNames-exportRVA)+exportOffset]; //Conversion to Offset
        px(exportDirectory->AddressOfNames, "[AddressOfNames] Address of Names");
        px(nOfFunc, "[nOfFunc] Number of Functions");
        px((DWORD)addrFuncs, "[addrFuncs] Address of Functions");

        strcpy(dllExt, vargs[1]);
        strcat(dllExt, ".ext");
        strcpy(dllSubs, vargs[1]);
        strcat(dllSubs, ".subs");
        fExt = fopen(dllExt,"w");
        fSubs = fopen(dllSubs, "w");
        
        printf("\n\tCreated ASM [extern] file> %s\n", dllExt);
        printf("\tCreated ASM functions file> %s\n", dllSubs);

        fprintf(fExt, "section .idata\n\n");
        fprintf(fSubs, "section .text\n\n");
        for(DWORD a=0; a<nOfFunc; a++){
            char *funcName = &buf[(addrFuncs[a]-exportRVA)+exportOffset];
            fprintf(fExt, "\textern __imp_%s\n", funcName);
            
            fprintf(fSubs, "%s:\n", funcName);
            fprintf(fSubs, "\tjmp [__imp_%s]\n", funcName);
            fprintf(fSubs, "\tretn\n\n");
        }
        
        fclose(fExt);
        fclose(fSubs);        
    }
    
    free(buf);

    return 0;
}