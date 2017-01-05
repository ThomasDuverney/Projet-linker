#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

char* lire_nom(Elf32_Ehdr structElf32, Elf32_Shdr structSectionHeader,int numSection,FILE* fichierElf);

char* AccesTableNomSection(Elf32_Ehdr elfHdr,FILE * fichierElf);

Elf32_Ehdr lireHeaderElf(char *argv[]);
