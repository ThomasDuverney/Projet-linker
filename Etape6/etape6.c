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

	ContenuElf * contenuElf = malloc(sizeof(ContenuElf));

	remplirStructure(fichDest,contenuElf);
	printf("%s",contenuElf->tabSections[5].nomSection);
	fclose(fichDest);
	fclose(secondFich);
	return 0;

}
