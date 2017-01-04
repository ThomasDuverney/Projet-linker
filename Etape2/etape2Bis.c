#include <stdio.h>
#include <stdlib.h>
#include <elf.h>


int main (int argc, char *argv[]){
  FILE* ElfFile = NULL;
  char* SectNames = NULL;
  Elf32_Ehdr elfHdr;
  Elf32_Shdr sectHdr;
  uint idx;

  ElfFile = fopen ( argv[1], "rb" );  
	
  // read ELF header
  fread(&elfHdr, 1, sizeof(elfHdr), ElfFile);

  // read section name string table
  // first, read its header
  fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof sectHdr, SEEK_SET); // Acces table des noms 
  fread(&sectHdr, 1, sizeof (sectHdr), ElfFile); // Lit la table des noms pour pouvoir faire le malloc de sectNames

  // next, read the section, string data
  SectNames = malloc(sectHdr.sh_size); 
  fseek(ElfFile, sectHdr.sh_offset, SEEK_SET); //REVIENT DEBUT TABLE DES NOMS
  fread(SectNames, 1, sectHdr.sh_size, ElfFile); // REMPLIT TABLEAU DES CARACTÈRES

  // read all section headers
  for (idx = 0; idx < elfHdr.e_shnum; idx++)
  {
    const char* name = "";

    fseek(ElfFile, elfHdr.e_shoff + idx * sizeof sectHdr, SEEK_SET);
    fread(&sectHdr, 1, sizeof sectHdr, ElfFile);

    // print section name
    if (sectHdr.sh_name);
      printf(" SectName %s\n",SectNames);
      printf(" Sh_name %c\n",sectHdr.sh_name);
      name = SectNames + sectHdr.sh_name;
    printf("%2u %s\n", idx, name);
    
    
  }


}
