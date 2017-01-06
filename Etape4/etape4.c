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
	
	printf("Table des symboles\nNuméro || Valeur ||Taille|| Type  ||Portée|| Nom\n");

	// On cherche la table des symboles
	Elf32_Shdr structSymTable = accesSectionParType(structElf32,fichierElf, SHT_SYMTAB);
	int symTableSize = structSymTable.sh_size/structSymTable.sh_entsize;	
	Elf32_Sym symbole;

	fseek(fichierElf,structSymTable.sh_offset,0); 

	for(int i=0; i<symTableSize; i++){ 
		fread(&symbole, sizeof(symbole), 1, fichierElf);
		// Affichage du numéro de section
		printf("  %d    ||", i);

		// Affichage de la valeur
		printf("%8.8x||", symbole.st_value);

		//Affichage de la taille
		printf("%2.2d    ||",symbole.st_size);
		
		// Affichage du type	
		switch(ELF32_ST_TYPE(symbole.st_info)){
					
						case 0:
							printf("NOTYPE ||");
							break;
						case 1:
							printf("OBJECT ||");
							break;
						case 2:
							printf("FUNC   ||");
							break;
						case 3:
							printf("SECTION||");
							break;
						case 4:
							printf("FILE   ||");
							break;
						case 13:
							printf("LOPROC ||");
							break;
						case 15:
							printf("HIPROC ||");
							break; 
						default:
							printf("Error  ||");
							break;
		}

	 //Affichage de la portée

		switch(ELF32_ST_BIND(symbole.st_info)){
					
								case 0:
									printf("LOCAL || ");
									break;
								case 1:
									printf("GLOBAL|| ");
									break;
								case 2:
									printf("WEAK  ||");
									break;
								case 13:
									printf("LOPROC||");
									break;
								case 15:
									printf("HIPROC||");
									break;
								default:
									printf("Error ||");
									break;
				}


		// Affichage du nom
		char* name = NULL;

		
		printf("%s",name);

		printf("\n");
		
	}

	return 0;
}
