#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "fonctionUtile.h"
#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t)) 

//<=> readelf -r file1.o


int main(int argc, char *argv[]){
	
	FILE* fichierElf = NULL;
	//Elf32_Ehdr structElf32;
	Elf32_Shdr structSectionHeader;
	Elf32_Rel structRelocation;
	int i;//,j;   //variable de boucle
    
	if(argc != 2){
    	printf("Syntaxe : ./etape2 file\n");
    	exit(1);
    }
    
    fichierElf = fopen ( argv[1], "rb" );  
    
    if (fichierElf==NULL) 
    {
    	printf ("\nFile error\n"); 
  		exit (1);
  	}


// ON APPELLE LA FONCTION QUI LIT LE HEADER DU FICHIER ELF
      Elf32_Ehdr structElf32 = lireHeaderElf(argv);


      //on se place au début de la table des headers
  	fseek(fichierElf,structElf32.e_shoff,0);







printf("Offset  ||Info       ||Type       || Sym Value    || Sym Name \n");

	// Pour chaque section header
	for(i = 0; i < structElf32.e_shnum; i++) {
		

	if (!fread(&structSectionHeader, sizeof(structSectionHeader), 1, fichierElf)){
    			printf("failed to read elf section header\n");
    			exit(1);
  		}
 
/*	// If this is a relocation section
	if(structSectionHeader.sh_type == SHT_REL) {
		// Process each entry in the table
		for(j = 0; j < structSectionHeader.sh_size / structSectionHeader.sh_entsize; j++) {
			StructRelocation = &((Elf32_Rel *)((int)hdr + section->sh_offset))[j];
				int result = elf_do_reloc(hdr, reltab, section);

*/






//Decalage
printf("%x||",structRelocation.r_offset);
//Info
printf("%x||",structRelocation.r_info);
//Type





printf("\n");
}



	return 0;	
}
