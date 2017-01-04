#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
    
    
int main (int argc, char *argv[]){
    
    FILE* fichierElf = NULL;
    Elf32_Ehdr structElf32;
    
    fichierElf = fopen ( argv[1], "rb" );  
    
    if (fichierElf==NULL) 
    {
        printf ("\nFile error"); 
        exit (1);
    }

    if (!fread(&structElf32, sizeof(structElf32), 1, fichierElf))
    {
            printf("failed to read elf header\n");
            exit(1);
    }

    printf("En-tête ELF : \n");
    
    
// ------------LA MACHINE 
    printf("Machine :\t");
        
    if(structElf32.e_machine == 1)
          printf("AT&T WE 32100\n");
    else if(structElf32.e_machine == 2)
       	  printf("SPARC\n");
    else if(structElf32.e_machine == 3)
    	  printf("Intel 80386\n");
    else if(structElf32.e_machine == 4)
    	  printf("Motorola 68000\n");
    else if(structElf32.e_machine == 5)
    	  printf("Motorola 88000\n");
    else if(structElf32.e_machine == 6)
    	  printf("Intel 80860\n");
    else if(structElf32.e_machine == 64)
    	  printf("AMD\n");
    else
        printf("Inconnu ou non spécifie\n");	
          
// -----------LE TYPE 
    printf("Type :\t");   	  
    if(structElf32.e_type == 1)
        printf("Relocatable\n");
    else if(structElf32.e_type == 2)
        printf("Executable\n");
    else if(structElf32.e_type == 3)
        printf("Shared Object\n");
    else if(structElf32.e_type == 0)
        printf("No file type\n");
    else
        printf("Inconnu ou non spécifie\n");
        
        
// -----------LA TAILLE DU HEADER
    printf("Taille :\t%",structElf32.e_phentsize);   	  


    fclose(fichierElf);
    
    return 0;

}


