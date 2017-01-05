#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"



int main(int argc, char *argv[]){
	
	FILE* fichierElf = NULL;
    
	if(argc != 2){
    	printf("Syntaxe : ./etape2 file\n");
    	exit(1);
     }
    
    fichierElf = fopen ( argv[1], "rb" );  
    
    if (fichierElf==NULL) {
    	printf ("\nFile error\n"); 
  		exit (1);
  	}

    // ON APPELLE LA FONCTION QUI LIT LE HEADER DU FICHIER ELF
    Elf32_Ehdr structElf32 = lireHeaderElf(argv);
        

   Elf32_Shdr * tabHeaders = accesTableDesHeaders(structElf32,fichierElf);

/*	int j;
	for(j=0;j<structElf32.e_shnum; j++){
	
		printf("%s \n",lire_nom(structElf32,j,fichierElf));
		printf("%i \n",tabHeaders[j].sh_size);
		printf("%i \n",tabHeaders[j].sh_offset);
	}*/
	
	
	
	Elf32_Shdr tempHed = RechercheSectionByName(fichierElf,".shstrtab",tabHeaders,structElf32);

	afficheSection(tempHed.sh_offset,tempHed.sh_size,fichierElf);
	
	return 0;
	
}


