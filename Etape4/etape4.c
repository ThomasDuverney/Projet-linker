#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"

int main(int argc, char *argv[]){
	FILE* fichierElf = NULL;

	if(argc != 2){
  	printf("Syntaxe : ./etape4 file\n");
  	exit(1);
  }

	fichierElf = fopen ( argv[1], "rb" );

  if (fichierElf==NULL) 
  {
  	printf ("\nFile error\n"); 
		exit (1);
	}

  // On appelle la fonction qui lit le header du fichier ELF
  Elf32_Ehdr structElf32 = lireHeaderElf(argv);

  //On se place au début de la table des headers
	fseek(fichierElf,structElf32.e_shoff,0); 
	
	printf("Table des symboles\nNuméro || Valeur || Type || Portée || Nom\n");

	// On cherche la table des symboles
	Elf32_Shdr structSymTable = accesSectionTableSymboles(structElf32,fichierElf);
	int symTableSize = structSymTable.sh_size/structSymTable.sh_entsize;	
	Elf32_Sym symbole;

	for(int i=0; i<symTableSize; i++){ 
		fread(&symbole, sizeof(symbole), 1, fichierElf);
		// Affichage du numéro de section
		printf("%i ||", i);

		// Affichage de la valeur
		printf("%i ||", symbole.st_value);
		
		// Affichage du type
		switch(symbole.st_info){
			
		}

		printf("\n");
		
	}

	return 0;
}
