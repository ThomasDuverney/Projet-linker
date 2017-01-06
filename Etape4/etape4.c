#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"

char * AccesTableString(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf);

char * LireNomSymb(char * tabString, int indexSymb);

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
  	Elf32_Ehdr structElf32 = lireHeaderElf(fichierElf);


	printf("Table des symboles\nNuméro || Valeur ||Taille|| Type  ||Portée|| Nom\n");

	// On cherche la table des symboles
	Elf32_Shdr structSymTable;
	structSymTable = RechercheSectionByName(fichierElf,".symtab",tabHeaders,structElf32);
	
	int symTableSize = structSymTable.sh_size/structSymTable.sh_entsize;	
	Elf32_Sym symbole;
	
	Elf32_Shdr * tabHeaders = accesTableDesHeaders(structElf32,fichierElf);
	
	
	char * tabString;
	int size;
	
	tabString = AccesTableString(tabHeaders,structElf32,&size,fichierElf);

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
	
		if (symbole.st_name){
			char * nomSymb = LireNomSymb(tabString,symbole.st_name);		
			printf("%s",nomSymb);
		}	
		printf("\n");
		
	}

	return 0;
}

char * AccesTableString(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf){

	Elf32_Shdr HdrTableString;
	
	HdrTableString = RechercheSectionByName(fichierElf,".strtab",tabHeaders,structElf32);
	
	Elf32_Off position = HdrTableString.sh_offset;
	Elf32_Word  taille = HdrTableString.sh_size;
	*size=taille;
	
	char * tabString = malloc(taille); 
	
	if( tabString == NULL ){
		return NULL;
	}
	
	fseek(fichierElf,position, SEEK_SET); 
  	fread(tabString,1,taille,fichierElf);
  	
  	return tabString;
	
	// RECHERCHE TABLE STRING .STRTAB
	// LA REMPLIR GRACE A SON OFFSET ET SA TAILLE DANS UN TABLEAU CHAR AVEC FREAD
	// UTILISER LE TABLEAU + INDEX SYMBOLE POUR TROUVER LE NOM DU SYMBOLE
}

char * LireNomSymb(char * tabString, int indexSymb){

	char * nomSymb;
	nomSymb = tabString + indexSymb;
	
	return nomSymb;
}
