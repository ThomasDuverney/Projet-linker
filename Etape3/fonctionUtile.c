#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "fonctionUtile.h"

char* lire_nom(Elf32_Ehdr structElf32, Elf32_Shdr structSectionHeader,int numSection,FILE* fichierElf){
     char* name = "";
     
     char* TableNomSection = AccesTableNomSection(structElf32,fichierElf);
	
    fseek(fichierElf, structElf32.e_shoff + numSection * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&structSectionHeader, 1, sizeof(Elf32_Shdr), fichierElf);

    // print section name
    if (structSectionHeader.sh_name)
      name = TableNomSection + structSectionHeader.sh_name;
    
    return name;
    
}

char* AccesTableNomSection(Elf32_Ehdr elfHdr,FILE * fichierElf){


  char* tabNomSection = NULL;
  Elf32_Shdr sectHdr;

   // read section name string table
  // first, read its header
  fseek(fichierElf, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof sectHdr, SEEK_SET);
  fread(&sectHdr, 1, sizeof (sectHdr), fichierElf);

  // next, read the section, string data
  tabNomSection = malloc(sectHdr.sh_size);
  fseek(fichierElf, sectHdr.sh_offset, SEEK_SET); //REVIENT DEBUT SECTION
  fread(tabNomSection, 1, sectHdr.sh_size, fichierElf);
  
  return tabNomSection;
  
}

Elf32_Ehdr lireHeaderElf(char *argv[]){

    FILE* fichierElf = NULL;
    Elf32_Ehdr structElf32;

    fichierElf = fopen ( argv[1], "rb" );  
    
    if (fichierElf==NULL) 
    {
        printf ("\nFile error"); 
        exit (1);
    }

    if (!fread(&structElf32, sizeof(structElf32), 1, fichierElf))
    {
            printf("failed to read elf header\n");
            exit(1);
    }
    
    return structElf32;
}
