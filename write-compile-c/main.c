#include <stdio.h>

#define ERRO "\tNot valid!"
#define SUCCESS "This program is valid!"

int valid_key(char* key){

    char validkey[11] = "321-123-231";
    int i,size;

    size = 0;    
    for(i=0;!key[i];i++){
        size++;
    }

    if(size > 11){
        printf("%s", ERRO);
        return 0;
    }

    for(i=0;i<sizeof(validkey);i++){
        if(validkey[i]!=key[i]){
            printf("\t%s\n", ERRO);
            return 0;
        }
    }

    return 1;

}

void main_program(){
    printf("You reach the secret stuff!!");
}

int main(int arg, char* argsv[]){

    char* typedkey; //Careful when defining pointer with no defined size.

    if(arg <= 1){
        printf("\t%s\n", ERRO);
    } else {
        printf("Type the password: ");
        scanf("%s", typedkey);
        if(valid_key(typedkey)==1){
            main_program(typedkey);
        }
    }

}