#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#include "phase1.h"

int fusionTabSymLocaux(ContenuElf * contenuElf,ContenuElf * contenuFusion);
void updateIndexSymb(int lastLocauxAdd,ContenuFus * contenuFus);
void fusionTabSymGlobal(int firstGlobal_1,int firstGlobal_2,ContenuFus * contenuFus);
void ajoutTabSymb(ContenuElf * contenuFusion,Elf32_Sym symbAdd);

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
		contenuFus->contenuElfFinal->tableString = (char *) contenuFus->contenuElfFinal->tabSections[i].contenuSection;
	}

  contenuFus->contenuElfFinal->tabSymb = malloc(sizeof(Elf32_Sym));
	contenuFus->contenuElfFinal->symTableSize = 0;

	int firstGlobal_1 = fusionTabSymLocaux(contenuFus->contenuElf1,contenuFus->contenuElfFinal);
  int firstGlobal_2 = fusionTabSymLocaux(contenuFus->contenuElf2,contenuFus->contenuElfFinal);
  updateIndexSymb(firstGlobal_1,contenuFus);
  fusionTabSymGlobal(firstGlobal_1,firstGlobal_2,contenuFus);

	printf("AFFICHAGE TABLE F1\n");
  fonctionEtape4(contenuFus->contenuElf1->tabSymb,contenuFus->contenuElf1->symTableSize,contenuFus->contenuElf1->tableString);

  printf("AFFICHAGE TABLE F2\n");
  fonctionEtape4(contenuFus->contenuElf2->tabSymb,contenuFus->contenuElf2->symTableSize,contenuFus->contenuElf2->tableString);

	printf("AFFICHAGE TABLE FUSION\n");
  fonctionEtape4(contenuFus->contenuElfFinal->tabSymb,contenuFus->contenuElfFinal->symTableSize,contenuFus->contenuElfFinal->tableString);


 // FAIRE FUSION LOCAUX && CHANGER LES OFFSET DANS SH_NAME SUR LE FICHIER 2
 // FAIRE FUSION GLOBAUX
 // CHANGER LES PARAMETRES DE L'ETAPE4

	/*
	// Affichage des sections
	printf("\x1b[34mAffichage des sections du fichier 1 \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElf1);void fusionTabSymGlobal(int firstGlobal,ContenuFus * contenuFus)

	printf("\x1b[34mAffichage des sections du fichier fusionné \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElfFinal);*/

	fclose(fichDest);
	fclose(secondFich);
	return 0;

}

int fusionTabSymLocaux(ContenuElf * contenuElf,ContenuElf * contenuFusion){
	Elf32_Sym * tabSymbFile = contenuElf->tabSymb;
	int sizeTabFile = contenuElf->symTableSize;
	int * sizeTableSym = &(contenuFusion->symTableSize);

	int i =0;

	while( i<sizeTabFile && ELF32_ST_BIND(tabSymbFile[i].st_info) == STB_LOCAL){
		if((*sizeTableSym) > 0 ){
			Elf32_Sym * tabTemp = realloc(contenuFusion->tabSymb,sizeof(Elf32_Sym)*((*sizeTableSym)+1));
			if(tabTemp!=NULL){
					contenuFusion->tabSymb = tabTemp;
					contenuFusion->tabSymb[*sizeTableSym] = tabSymbFile[i];
					(*sizeTableSym)++;
			}
  	}else{
			contenuFusion->tabSymb[*sizeTableSym] = tabSymbFile[i];
			(*sizeTableSym)++;
		}
		i++;
 }

 return i;
}

void updateIndexSymb(int lastLocauxAdd,ContenuFus * contenuFus){
			Elf32_Sym ** tabSymbFile = &(contenuFus->contenuElfFinal->tabSymb);
			int sizeTabFile = contenuFus->contenuElfFinal->symTableSize;
			int i= lastLocauxAdd;

			for(; i<sizeTabFile ;i++){
				(*tabSymbFile)[i].st_name = (*tabSymbFile)[i].st_name + contenuFus->contenuElf1->sizeTabString;
			}
}


void fusionTabSymGlobal(int firstGlobal_1,int firstGlobal_2,ContenuFus * contenuFus){
	Elf32_Sym * tabSymbFile_1 = contenuFus->contenuElf1->tabSymb;
	int sizeTabFile_1 = contenuFus->contenuElf1->symTableSize;

	Elf32_Sym * tabSymbFile_2 = contenuFus->contenuElf2->tabSymb;
	int sizeTabFile_2 = contenuFus->contenuElf2->symTableSize;

	int * sizeTableSym = &(contenuFus->contenuElfFinal->symTableSize);
	int i= firstGlobal_1;
	int j= firstGlobal_2;
	char * nomSymb_1;
	char * nomSymb_2;

	int sizeWrited = 0;
	int flag ;
	int nbMaxGlobaux = (sizeTabFile_1 -firstGlobal_1) + (sizeTabFile_2 -firstGlobal_2);
	int * tabWrited = malloc(sizeof(int)*nbMaxGlobaux);

	for(; i<sizeTabFile_1 ;i++){

		  if( ELF32_ST_BIND(tabSymbFile_1[i].st_info) == STB_GLOBAL){
				j= firstGlobal_2;
			for(;j < sizeTabFile_2;j++){
				printf("symb %i\n",j);
				nomSymb_1 = LireNomSymb((char *)contenuFus->contenuElf1->tableString, tabSymbFile_1[i].st_name);
				nomSymb_2 = LireNomSymb((char *)contenuFus->contenuElf2->tableString, tabSymbFile_2[j].st_name);

				// SI LES SYMBOLES SONT GLOBAUX ET SONT DE MEME NOM
				if(strcmp(nomSymb_1,nomSymb_2) == 0){

					if(tabSymbFile_1[i].st_shndx != SHN_UNDEF && tabSymbFile_2[j].st_shndx != SHN_UNDEF){
							ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_1[i]);

					}else if(tabSymbFile_1[i].st_shndx != SHN_UNDEF && tabSymbFile_2[j].st_shndx == SHN_UNDEF){
						  ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_1[i]);


					}else if(tabSymbFile_1[i].st_shndx == SHN_UNDEF && tabSymbFile_2[j].st_shndx != SHN_UNDEF){
							ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_2[j]);
							contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name = contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name
																																	+ contenuFus->contenuElf1->sizeTabString;


					}else{
						printf("Deux Symboles globaux défini avec le même nom sont présents dans la table des entrées\n");
						printf("Echec de l'édition des liens\n");
						exit(1);
					}
					flag =1;
					tabWrited[sizeWrited] = j;
					printf("JE test %i\n",tabWrited[sizeWrited]);
					sizeWrited++;
				}
			}
			if(flag == 0){
					ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_1[i]);
			}
		}
	}

	j= firstGlobal_2;

	for(;j< sizeTabFile_2;j++){
		i=0;
		while(i<sizeWrited && tabWrited[i] != j){
			printf("les numero de tabWrited %i\n",tabWrited[i]);
			i++;

		}
		if(sizeWrited == i){
				printf("JE PASSE %i\n",tabWrited[i]);
				ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_2[j]);
				contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name = contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name
																														+ contenuFus->contenuElf1->sizeTabString;
		}
	}
	free(tabWrited);
}

void ajoutTabSymb(ContenuElf * contenuFusion,Elf32_Sym symbAdd){

	int * sizeTableSym = &(contenuFusion->symTableSize);

	if((*sizeTableSym) > 0 ){
		Elf32_Sym * tabTemp = realloc(contenuFusion->tabSymb,sizeof(Elf32_Sym)*((*sizeTableSym)+1));
		if(tabTemp!=NULL){
				contenuFusion->tabSymb = tabTemp;
				contenuFusion->tabSymb[*sizeTableSym] = symbAdd;
				(*sizeTableSym)++;
		}
	}else{
		contenuFusion->tabSymb[*sizeTableSym] = symbAdd;
		(*sizeTableSym)++;
	}

}
