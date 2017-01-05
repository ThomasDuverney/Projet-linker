#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"


Elf32_Shdr *  rechercherTablesReimplentation(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf);

Elf32_Rel * tabSymboleRel(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);

Elf32_Rela * tabSymboleRela(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);


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
   	
// CREATION TABLEAU DES HEADERS SECTION REIMPLENTATION
	int size;
	Elf32_Shdr * tabReal;
	tabReal = rechercherTablesReimplentation(tabHeaders,structElf32, &size,fichierElf);

	int i;
	Elf32_Rel * tabSymRel;
		
// CREATION TABLEAU DES SYMBOLES REL POUR CHAQUE SECTION REL

	for(i =0 ; i < size; i++){
	
		tabSymRel =tabSymboleRel(tabReal[i].sh_offset,tabReal[i].sh_size,fichierElf);
	}
	
	
	int j;
	for(j =0 ; j <= tabReal[i].sh_size ; j++){
		printf("%i",tabSymRel[i].r_offset);
	}

	
	
	return 0;
	
}
/*
void fonctionEtape5(fichierElf,structElf32,tabHeaders){


}*/

Elf32_Shdr *  rechercherTablesReimplentation(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf){

	Elf32_Shdr * tabSectionReimp;
	*size=0;
	int j;
	 
	for(j=0;j<structElf32.e_shnum; j++){
		
		if(tabHeaders[j].sh_type == SHT_RELA || tabHeaders[j].sh_type == SHT_REL){
			if(*size == 0){
			
				tabSectionReimp=malloc(sizeof(Elf32_Shdr));
				if( tabSectionReimp == NULL ){
					return NULL;
				}
				else{
					tabSectionReimp[*size] = tabHeaders[j];
					(*size)++;
				}
			}else{
				Elf32_Shdr * tabTemp;
				tabTemp=realloc(tabSectionReimp,sizeof(Elf32_Shdr)*(*size+1));
				
				if ( tabTemp == NULL){
					return NULL;
				}else{
					tabSectionReimp=tabTemp;
					tabSectionReimp[*size] = tabHeaders[j];
					(*size)++;	
				}		
			}
			
		
		}
		
	}
	
	return tabSectionReimp;

}



Elf32_Rel * tabSymboleRel(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf){
	
	Elf32_Rel * tabSymbolRel = malloc(taille);
	Elf32_Rel  tempSymb;
	
	if(tabSymbolRel == NULL){
		printf("Erreur allocation dans le tableau des symboles de réalocation"); 
		return NULL;
	}
	
	int i;
	fseek(fichierElf,position, SEEK_SET);
	for(i=0; i< taille/sizeof(Elf32_Rel);i++){
	
		fread(&tempSymb,1,sizeof(Elf32_Rel),fichierElf);
		tabSymbolRel[i]=tempSymb;

	}
	
	return tabSymbolRel;
	
	
}

Elf32_Rela * tabSymboleRela(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf){
	
	Elf32_Rela * tabSymbolRela = malloc(taille);
	Elf32_Rela  tempSymb;
	
	if(tabSymbolRela == NULL){
		printf("Erreur allocation dans le tableau des symboles de réalocation"); 
		return NULL;
	}
	
	int i;
	fseek(fichierElf,position, SEEK_SET);
	for(i=0; i< taille/sizeof(Elf32_Rela);i++){
	
		fread(&tempSymb,1,sizeof(Elf32_Rel),fichierElf);
		tabSymbolRela[i]=tempSymb;
	}
	
	return tabSymbolRela;
	
}

