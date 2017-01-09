#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "fonctionUtile.h"

int main(int argc,char* argv[]){

	FILE* fichDest = NULL;
	FILE* secondFich = NULL;
	//int i;

	if(argc!=3){

		printf("Syntaxe : ./etape6 file1 file2\n");
		exit(1);

	}
	
	fichDest = ouvrirFichier(argv[1]);
	secondFich = ouvrirFichier(argv[2]);
	
	ContenuFus * contenuFus = malloc(sizeof(ContenuFus));
	
	contenuFus->ContenuElf1 = malloc(sizeof(ContenuElf));
	contenuFus->ContenuElf2 = malloc(sizeof(ContenuElf));
	
	remplirStructure(fichDest,contenuFus->ContenuElf1);
	remplirStructure(secondFich,contenuFus->ContenuElf2);
	
	printf("%s",contenuFus->ContenuElf1->tabSections[5].nomSection);

	fclose(fichDest);
	fclose(secondFich);
	return 0;

}
