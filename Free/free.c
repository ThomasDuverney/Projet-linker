#include <stdio.h>
#include <stdlib.h>
#include "fonctionUtile.h"
#include <elf.h>
#include <string.h>

void libererMemoire(ContenuFus * contenuFus);
void libererContenuElf(ContenuElf * contenuElf);
void libererTabHeaders(Elf32_Shdr * TabHeaders);

int main (int argc, char *argv[]){

  	FILE* fichierElf;
	Elf32_Shdr * TabHeaders = NULL;
	ContenuFus * contenuFus = malloc(sizeof(ContenuFus));
	contenuFus->contenuElf1 = malloc(sizeof(ContenuElf));
	contenuFus->contenuElf2 = NULL;
	contenuFus->contenuElfFinal = NULL;
	
	fichierElf = ouvrirFichier(argv[1]);
	remplirStructure(fichierElf,contenuFus->contenuElf1,&TabHeaders);

	libererMemoire(contenuFus);	
	libererTabHeaders(TabHeaders);

		// Fermeture du fichier
		fclose(fichierElf);

return 0;
}

void libererTabHeaders(Elf32_Shdr * TabHeaders){
	
	free(TabHeaders);
	printf("\nMémoire libérée (TabHeaders)\n");
	
}

void libererMemoire(ContenuFus * contenuFus){

	if(contenuFus->contenuElf1 != NULL){
		
		libererContenuElf(contenuFus->contenuElf1);
		
	}
	
	if(contenuFus->contenuElf2 != NULL){
		
		libererContenuElf(contenuFus->contenuElf2);
		
	}
	
	if(contenuFus->contenuElfFinal != NULL){
		
		libererContenuElf(contenuFus->contenuElfFinal);
	
	}
	free(contenuFus);
	
	printf("\nMémoire libérée (ContenuFus)\n");
	
}

void libererContenuElf(ContenuElf * contenuElf){

	int i;
	
	free(contenuElf->tableString);
	free(contenuElf->TableNomSection);
	free(contenuElf->tabSymb);
	free(contenuElf->tabRela);
	//printf("%i\n",contenuElfFinal->hdrElf.e_shnum);
	
	for (i=0;i<contenuElf->hdrElf.e_shnum;i++){
		free((contenuElf->tabSections)[i].contenuSection);
	}	
	free(contenuElf->tabSections);		
	free(contenuElf);
	
}
