#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"
void fusionTabSymLocaux(ContenuElf * contenuElf,ContenuElf * contenuFusion);
int main(int argc,char* argv[]){

	FILE* fichDest = NULL;
	FILE* secondFich = NULL;
	int size1,size2;

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

	Elf32_Shdr * TabHeaders1 = NULL;
	Elf32_Shdr * TabHeaders2 = NULL;

	remplirStructure(fichDest,contenuFus->contenuElf1,&TabHeaders1);
	remplirStructure(secondFich,contenuFus->contenuElf2,&TabHeaders2);



	SectionInfos * tabSectionProgb1;
	SectionInfos * tabSectionProgb2;

	tabSectionProgb1 = RechercheSectionByType(SHT_STRTAB,&size1,contenuFus->contenuElf1);
	tabSectionProgb2 = RechercheSectionByType(SHT_STRTAB,&size2,contenuFus->contenuElf2);

	fusionSection(tabSectionProgb1,tabSectionProgb2,size1,size2,contenuFus);

	int i;
	int sizetabsec = contenuFus->contenuElfFinal->sizeSections;
	while(i<sizetabsec && strcmp(contenuFus->contenuElfFinal->tabSections[i].nomSection,".strtab")){
		i++;
	}
	if(i!=sizetabsec){
		contenuFus->contenuElfFinal->tableString = contenuFus->contenuElfFinal->tabSections[i].contenuSection;
	}

	fusionTabSymLocaux(contenuFus->contenuElf1,contenuFus->contenuElfFinal);
  fusionTabSymLocaux(contenuFus->contenuElf2,contenuFus->contenuElfFinal);

 //fonctionEtape4(fichierElf,structElf32,tableNomSection,tabHeaders,tabSymb,symTableSize,tabString){

 
 // FAIRE FUSION LOCAUX && CHANGER LES OFFSET DANS SH_NAME SUR LE FICHIER 2
 // FAIRE FUSION GLOBAUX
 // CHANGER LES PARAMETRES DE L'ETAPE4

	/*
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

void fusionTabSymLocaux(ContenuElf * contenuElf,ContenuElf * contenuFusion){
	Elf32_Sym * tabSymbFile = contenuElf->tabSymb;
	contenuFusion->tabSymb = malloc(sizeof(Elf32_Sym));

	int sizeTabFile1 = contenuElf->symTableSize;
	contenuFusion->symTableSize = 0;
	int i =0;

	while( i<sizeTabFile1 && tabSymbFile[i].st_info == STB_LOCAL){
		if(contenuFusion->symTableSize !=0 ){
			Elf32_Sym * tabTemp = realloc(contenuFusion->tabSymb,sizeof(Elf32_Sym)*(contenuFusion->symTableSize+1));
			if(tabTemp!=NULL){
					contenuFusion->tabSymb = tabTemp;
					contenuFusion->symTableSize++;
			}
  	}
		i++;
 }
}
