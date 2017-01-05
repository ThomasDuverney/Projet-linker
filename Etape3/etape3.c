#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include <ctype.h>
#include "fonctionUtile.h"

void fonctionEtape3(FILE * fichierElf,char * section,Elf32_Ehdr structElf32, Elf32_Shdr * tabHeaders);

int main(int argc, char *argv[]){
	
	FILE* fichierElf = NULL;
    
	if(argc != 2){
    	printf("Syntaxe : ./etape3 file\n");
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
	fonctionEtape3(fichierElf,"5",structElf32,tabHeaders);
	
	
	/*
	int j;
	for(j=0;j<structElf32.e_shnum; j++){
	
		printf("%s \n",lire_nom(structElf32,j,fichierElf));
		printf("%i \n",tabHeaders[j].sh_size);
		printf("%i \n",tabHeaders[j].sh_offset);
	}*/
	
	return 0;
	
}

void fonctionEtape3(FILE * fichierElf,char * section,Elf32_Ehdr structElf32, Elf32_Shdr * tabHeaders){
	Elf32_Shdr tempHed;
	printf("Affichage de la section : %s \n",section);
	
	if(isalpha(*section) != 0){
	
		tempHed = RechercheSectionByName(fichierElf,section,tabHeaders,structElf32);
		afficheSection(tempHed.sh_offset,tempHed.sh_size,fichierElf);
	
	}else{
		
		Elf32_Shdr tempHed = tabHeaders[atoi(section)];
		afficheSection(tempHed.sh_offset,tempHed.sh_size,fichierElf);
		
	}
	
}  


