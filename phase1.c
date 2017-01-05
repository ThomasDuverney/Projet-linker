#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"

void fonctionEtape5(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,FILE * fichierElf){

	int size;
	Elf32_Shdr * tabReal;
	int i;
	int j;
	Elf32_Rel * tabSymRel; // Tableau symbole relocation rel
	Elf32_Rela * tabSymRela;// Tableau symbole relocation rela

	// CREATION TABLEAU DES HEADERS SECTION REIMPLENTATION
	tabReal = rechercherTablesReimplentation(tabHeaders,structElf32, &size,fichierElf);

	if(size!=0){

		// CREATION TABLEAU DES SYMBOLES POUR CHAQUE SECTION REALOCATION
		for(i =0 ; i < size; i++){

			if(tabReal[i].sh_type == SHT_REL){

					tabSymRel=tabSymboleRel(tabReal[i].sh_offset,tabReal[i].sh_size,fichierElf);


					for(j =0 ; j < tabReal[i].sh_size/sizeof(Elf32_Rel) ; j++){
						afficherRelocation(tabSymRel[j].r_info,tabSymRel[j].r_offset);
					}
			}
			else{
					tabSymRela=tabSymboleRela(tabReal[i].sh_offset,tabReal[i].sh_size,fichierElf);

					int j;
					for(j =0 ; j < tabReal[i].sh_size/sizeof(Elf32_Rel) ; j++){
						afficherRelocation(tabSymRela[j].r_info,tabSymRela[j].r_offset);
					}
			}
		}
	}else{
			printf("Ce fichier ne contient aucun repositionnement\n");
			exit(1);
	}
}
