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
		
	int i,k;
	contenuFus->contenuElfFinal->sizeSections = 0;

	for(k=0;k<size1;k++){
		
		for(i=0;i<size2;i++){
		
			if(strcmp(tabSectionProgb1[k].nomSection,tabSectionProgb2[i].nomSection) == 0){
	
				//concatène et ecrit dans le header de la section progb1 la nouvelle taille
				CopieSectionInfos(contenuFus,tabSectionProgb1[k]);
	
			}else{
				CopieSectionInfos(contenuFus,tabSectionProgb1[k]);
			}
		}
		
	}
/*
	for(k=0;k<size2;k++){
	
		for(i=0;i<size1;i++){
		
			if(strcmp(tabSectionProgb1[k].nomSection,tabSectionProgb2[i].nomSection) != 0){
				
				//Ajout de la nouvelle section et nouveau header section dans le contenuElf3 avec tabSectionProgb2[k]
				
			}
		}
		
	}*/

	printf("première :%s",contenuFus->contenuElfFinal->tabSections[0].nomSection);
	printf(" deuxième : %s",contenuFus->contenuElfFinal->tabSections[1].nomSection);
	//printf("%s",contenuFus->contenuElfFinal->tabSections[2].nomSection);
	//printf("%s",contenuFus->contenuElfFinal->tabSections[3].nomSection);
	//printf("%s",contenuFus->contenuElfFinal->tabSections[4].nomSection);
	
	fclose(fichDest);
	fclose(secondFich);
	return 0;

}
/*
void EcrireSection(ContenuElf * contenuElf,int numSection){

       
       fwrite(contenuElf->tabSections[numSection].contenuSection,1,contenuElf->tabSections.tabHdrSections.sh_size,contenuElf->fichierElf);
       


}*/


// Parcours les PROGBITS 


void CopieSectionInfos(ContenuFus * contenuFus,SectionInfos sectionInfos){

				
				if(contenuFus->contenuElfFinal->sizeSections == 0){
					contenuFus->contenuElfFinal->tabSections = malloc(sizeof(SectionInfos));
					if( contenuFus->contenuElfFinal->tabSections  != NULL){
						memcpy(&(contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections]),&sectionInfos,sizeof(SectionInfos));
						contenuFus->contenuElfFinal->sizeSections++;
						printf("Je rentre une fois");
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
				
				printf("%i",contenuFus->contenuElfFinal->sizeSections);
}	
/*
void ConcaContenuSection(char * contenuSection1,char * contenuSection2){
	
}*/
