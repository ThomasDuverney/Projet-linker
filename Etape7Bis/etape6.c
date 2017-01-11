#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"

int main(int argc,char* argv[]){

	FILE* fichDest = NULL;
	FILE* secondFich = NULL;
	//int size1,size2;

	if(argc!=3){

		printf("Syntaxe : ./etape6 file1 file2\n");
		exit(1);

	}

	fichDest = ouvrirFichier(argv[1]);
	secondFich = ouvrirFichier(argv[2]);

	ContenuFus * contenuFus = malloc(sizeof(ContenuFus));

	contenuFus->contenuElf1 = malloc(sizeof(ContenuElf));
	contenuFus->contenuElf2 = malloc(sizeof(ContenuElf));
	contenuFus->contenuElfFinal = malloc(sizeof(ContenuElf));

	/*Elf32_Shdr * TabHeaders1 = NULL;
	Elf32_Shdr * TabHeaders2 = NULL;

	remplirStructure(fichDest,contenuFus->contenuElf1,&TabHeaders1);
	//fonctionEtape2(contenuFus->contenuElf1->hdrElf,fichDest,contenuFus->contenuElf1->TableNomSection,TabHeaders1);
		//afficherVerifFusion(contenuFus->contenuElf1);
	remplirStructure(secondFich,contenuFus->contenuElf2,&TabHeaders2);
//	fonctionEtape2(contenuFus->contenuElf2->hdrElf,secondFich,contenuFus->contenuElf2->TableNomSection,TabHeaders2);


	SectionInfos * tabSectionProgb1;
	SectionInfos * tabSectionProgb2;

	tabSectionProgb1 = RechercheSectionByType(SHT_PROGBITS,&size1,contenuFus->contenuElf1);
	tabSectionProgb2 = RechercheSectionByType(SHT_PROGBITS,&size2,contenuFus->contenuElf2);

	fusionSection(tabSectionProgb1,tabSectionProgb2,size1,size2,contenuFus);

	// Affichage des sections
	printf("\x1b[34mAffichage des sections du fichier 1 \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElf1);

	printf("\x1b[34mAffichage des sections du fichier 2 \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElf2);

	printf("\x1b[34mAffichage des sections du fichier fusionné \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElfFinal);*/

	fclose(fichDest);
	fclose(secondFich);
	return 0;

}
