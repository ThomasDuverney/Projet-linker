#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"

void CopieSectionInfos(ContenuFus * contenuFus,SectionInfos sectionInfos);
void fusionSection(SectionInfos * tabSection1,SectionInfos * tabSection2,int size1, int size2,ContenuFus * contenuFus);
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
	
	fusionSection(tabSectionProgb1,tabSectionProgb2,size1,size2,contenuFus);
	
	
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
}

void fusionSection(SectionInfos * tabSection1,SectionInfos * tabSection2,int size1, int size2,ContenuFus * contenuFus){

	int i,k,flag;
	int sizeWrited = 0;
	int newSectionSize = 0;
	int * tabWrited = malloc(size2+size1);


	contenuFus->contenuElfFinal->sizeSections = 0;

	for(k=0;k<size1;k++){

		for(i=0;i<size2;i++){

			//printf("%s\n",tabSection[i].nomSection);
			if(strcmp(tabSection1[k].nomSection,tabSection2[i].nomSection) == 0){
				//concatène et ecrit dans le header de la section progb1 la nouvelle taille
				CopieSectionInfos(contenuFus,tabSection1[k]);
			 	newSectionSize = tabSection1[k].tabHdrSections.sh_size + tabSection2[i].tabHdrSections.sh_size;
			    unsigned char * tabTemp = realloc(tabSection1[k].contenuSection,newSectionSize);
			  
				if(tabTemp!=NULL){

					int j;
					for(j=0;j<tabSection2[i].tabHdrSections.sh_size;j++){
							tabTemp[tabSection1[k].tabHdrSections.sh_size+j]=tabSection2[i].contenuSection[j];
					}
					contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections-1].contenuSection = tabTemp;
					contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections-1].tabHdrSections.sh_size = newSectionSize;			
					flag = 1;
					tabWrited[sizeWrited]=i;
					sizeWrited++;

			  }
			}
		}
		if(flag == 0){
					CopieSectionInfos(contenuFus,tabSection1[k]);
		}

	}
	for(k=0;k<size2;k++){
		i=0;
		while(i<sizeWrited && tabWrited[i] != k){
			i++;
		}
		if(sizeWrited == i){
			CopieSectionInfos(contenuFus,tabSection2[k]);
		}
	}
	free(tabWrited);
}

void afficherEtape6(ContenuFus * contenuFus){

}
