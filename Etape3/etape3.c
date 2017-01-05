#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"

//<=> readelf -S file1.o
Elf32_Shdr * accesTableDesHeaders(Elf32_Ehdr structElf32, FILE * fichierElf);

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

	int j;
	for(j=0;j<structElf32.e_shnum; j++){
	
		printf("%s \n||",lire_nom(structElf32,tabHeaders[j],j,fichierElf));
	}
	
	
	return 0;
	
}
/*
void AffichageContenuSection(FILE * fichierElf, */

/*
Elf32_Shdr * accesTableDesHeaders(Elf32_Ehdr structElf32, FILE * fichierElf){

	Elf32_Shdr * tabHeaders = malloc(sizeof(Elf32_Shdr)*structElf32.e_shoff);
	Elf32_Shdr structSectionHeader;
	int i;
	
	fseek(fichierElf,structElf32.e_shoff,0); 
   	
   	//structElf32.e_shentsize <=> taille de la table des sections

  	for(i=0;i<structElf32.e_shnum;i++){
  	
  		if (!fread(&structSectionHeader, sizeof(Elf32_Shdr), 1, fichierElf))
  		{
    		printf("failed to read elf section header\n");
    		exit(1);
  		}
  		tabHeaders[i]=structSectionHeader;

  	}

	return tabHeaders;
}*/
