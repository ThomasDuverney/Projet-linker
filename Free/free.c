#include <stdio.h>
#include <stdlib.h>
#include "fonctionUtile.h"
#include <elf.h>
#include <string.h>

void LibererMemoire(ContenuFus * contenuFus,Elf32_Shdr * TabHeaders);

int main (int argc, char *argv[]){

  	FILE* fichierElf;
	Elf32_Shdr * TabHeaders = NULL;
	ContenuFus * contenuFus = malloc(sizeof(ContenuFus));
	contenuFus->contenuElf1 = malloc(sizeof(ContenuElf));
	
	fichierElf = ouvrirFichier(argv[1]);
	remplirStructure(fichierElf,contenuFus->contenuElf1,&TabHeaders);

	LibererMemoire(contenuFus,TabHeaders);	

		// Fermeture du fichier
		fclose(fichierElf);

return 0;
}

void LibererMemoire(ContenuFus * contenuFus,Elf32_Shdr * TabHeaders){

int i;
	if(contenuFus->contenuElf1 != NULL){
		free(contenuFus->contenuElf1->tableString);
		free(contenuFus->contenuElf1->TableNomSection);
		free(contenuFus->contenuElf1->tabSymb);
		free(contenuFus->contenuElf1->tabRela);
		free(TabHeaders);
		printf("%i\n",contenuFus->contenuElf1->hdrElf.e_shnum);
		
		for (i=0;i<contenuFus->contenuElf1->hdrElf.e_shnum;i++){
			free((contenuFus->contenuElf1->tabSections)[i].contenuSection);
		}	
		free(contenuFus->contenuElf1->tabSections);		
		free(contenuFus->contenuElf1);
		
		
	}
	
	if(contenuFus->contenuElf2 != NULL){
		
		free(contenuFus->contenuElf2->tableString);
		free(contenuFus->contenuElf2->TableNomSection);
		free(contenuFus->contenuElf2->tabSymb);
		free(contenuFus->contenuElf2->tabRela);
		free(TabHeaders);
		printf("%i\n",contenuFus->contenuElf2->hdrElf.e_shnum);
		
		for (i=0;i<contenuFus->contenuElf2->hdrElf.e_shnum;i++){
			free((contenuFus->contenuElf2->tabSections)[i].contenuSection);
		}	
		free(contenuFus->contenuElf2->tabSections);		
		free(contenuFus->contenuElf2);
		
		
	}
	
	if(contenuFus->contenuElfFinal != NULL){
		
		free(contenuFus->contenuElfFinal->tableString);
		free(contenuFus->contenuElfFinal->TableNomSection);
		free(contenuFus->contenuElfFinal->tabSymb);
		free(contenuFus->contenuElfFinal->tabRela);
		free(TabHeaders);
		printf("%i\n",contenuFus->contenuElfFinal->hdrElf.e_shnum);
		
		for (i=0;i<contenuFus->contenuElfFinal->hdrElf.e_shnum;i++){
			free((contenuFus->contenuElfFinal->tabSections)[i].contenuSection);
		}	
		free(contenuFus->contenuElfFinal->tabSections);		
		free(contenuFus->contenuElfFinal);
	
	}
free(contenuFus);
}
