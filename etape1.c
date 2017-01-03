#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
    
    
int main (int argc, char *argv[]){

  /*  FILE* fichier = NULL;

    fichier = fopen("test.txt", "rb");

    if (fichier != NULL)
    {
	  if (1 != fread(&hdr, sizeof(hdr), 1, ElfFile)){
            
            printf("failed to read elf header\n");
            exit(1);
    	  }
        
          fclose(fichier); // On ferme le fichier qui a été ouvert
    }*/
    
    printf ("Main()");
    FILE* ElfFile = NULL;
    Elf32_Ehdr hdr;

    ElfFile = fopen ( "file1.o" , "rb" );  
    if (ElfFile==NULL) 
    {
        printf ("\nFile error"); 
        exit (1);
    }

    if (1 != fread(&hdr, sizeof(hdr), 1, ElfFile))
    {
            printf("failed to read elf header\n");
            exit(1);
    }

		if(hdr.e_type == 1)
        printf("Relocatable\n");
    else if(hdr.e_type == 2)
        printf("Executable\n");
    else if(hdr.e_type == 3)
        printf("Shared Object\n");
    else
        printf("Unknown\n");


    return 0;

}


