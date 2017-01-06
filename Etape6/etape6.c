#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "fonctionUtile.h"

int main(int argc,char* argv[]){
	
	FILE* fichDest = NULL;
	FILE* secondFich = NULL;
	
	if(argc!=3){
		
		printf("Syntaxe : ./etape6 file1 file2\n");
		exit(1);
		
	}
	
	fichDest = fopen(argv[1], "rb");
	secondFich = fopen(argv[2], "rb");
	
	if(fichDest == NULL || secondFich == NULL){
		
		if(fichDest == NULL)
			printf("Erreur lors de l'ouverture du premier fichier\n");
		else
			printf("Erreur lors de l'ouverture du second fichier\n");
		exit(2);
		
	}
	
	//Lecture des headers
	Elf32_Ehdr structDest = lireHeaderElf(argv[1]);
	Elf32_Ehdr structSecond = lireHeaderElf(argv[2]);
	
	
	fclose(fichDest);
	fclose(secondFich);
	return 0;
	
}
