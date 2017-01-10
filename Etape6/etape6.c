#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"

void CopieSectionInfos(ContenuFus * contenuFus,SectionInfos sectionInfos);

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

	remplirStructure(fichDest,contenuFus->contenuElf1);
	remplirStructure(secondFich,contenuFus->contenuElf2);

	SectionInfos * tabSectionProgb1;
	SectionInfos * tabSectionProgb2;

	tabSectionProgb1 = RechercheSectionByType(SHT_PROGBITS,&size1,contenuFus->contenuElf1);
	tabSectionProgb2 = RechercheSectionByType(SHT_PROGBITS,&size2,contenuFus->contenuElf2);

	int i,k,flag = 0;
	int sizeWrited =0;
	int newSectionSize = 0;
	int * tabWrited = malloc(size2+size1);


	contenuFus->contenuElfFinal->sizeSections = 0;

	for(k=0;k<size1;k++){

		for(i=0;i<size2;i++){

			//printf("%s\n",tabSectionProgb2[i].nomSection);
			if(strcmp(tabSectionProgb1[k].nomSection,tabSectionProgb2[i].nomSection) == 0){
				//concatène et ecrit dans le header de la section progb1 la nouvelle taille
				CopieSectionInfos(contenuFus,tabSectionProgb1[k]);
			 	newSectionSize = tabSectionProgb1[k].tabHdrSections.sh_size + tabSectionProgb2[i].tabHdrSections.sh_size;
			    char * tabTemp = realloc(tabSectionProgb1[k].contenuSection,newSectionSize);
			  
				if(tabTemp!=NULL){

					int j;
					for(j=0;j<tabSectionProgb2[i].tabHdrSections.sh_size;j++){
							tabTemp[tabSectionProgb1[k].tabHdrSections.sh_size+j]=tabSectionProgb2[i].contenuSection[j];
					}
					contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections-1].contenuSection = tabTemp;
					int z;
					for(z=0;z<newSectionSize;z++){
						if(z%4 == 0 && z!=0){
							printf("     ");
						}
						if(z%16 == 0 && z!=0){
							printf("\n");
						}
						printf("%x",contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections-1].contenuSection[z]);
					}
						
					contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections-1].tabHdrSections.sh_shize = newSectionSize;
						//MODIFIER LA TAILLE DANS LE SECTION HEARDER
						// CHANGER LA FONCTION AFFICHE SECTION METTRE EN PARAMETRE UN CONTENU DE SECTION
						// PROBLEME AFFICHAGE SECTION
						exit(1);
					flag = 1;
					tabWrited[sizeWrited]=i;
					sizeWrited++;

			  }
			}
		}
		if(flag == 0){
					CopieSectionInfos(contenuFus,tabSectionProgb1[k]);
		}

	}
	for(k=0;k<size2;k++){
		i=0;
		while(i<sizeWrited && tabWrited[i] != k){
			i++;
		}
		if(sizeWrited == i){
			CopieSectionInfos(contenuFus,tabSectionProgb2[k]);
		}
	}
	free(tabWrited);

	for(i=0;i<contenuFus->contenuElfFinal->sizeSections;i++)
			printf("%s",contenuFus->contenuElfFinal->tabSections[i].nomSection);

	fclose(fichDest);
	fclose(secondFich);
	return 0;

}

void CopieSectionInfos(ContenuFus * contenuFus,SectionInfos sectionInfos){


				if(contenuFus->contenuElfFinal->sizeSections == 0){
					contenuFus->contenuElfFinal->tabSections = malloc(sizeof(SectionInfos));
					if( contenuFus->contenuElfFinal->tabSections  != NULL){
						memcpy(&(contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections]),&sectionInfos,sizeof(SectionInfos));
						contenuFus->contenuElfFinal->sizeSections++;
					}

				}else{
					SectionInfos * tabTemp;
					tabTemp=realloc(contenuFus->contenuElfFinal->tabSections,sizeof(SectionInfos)*(contenuFus->contenuElfFinal->sizeSections+1));

					if ( tabTemp != NULL){

						contenuFus->contenuElfFinal->tabSections=tabTemp;
						memcpy(&(contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections]),&sectionInfos,sizeof(SectionInfos));
						contenuFus->contenuElfFinal->sizeSections++;

					}
				}
			//	printf("%s",sectionInfos.nomSection);
}

