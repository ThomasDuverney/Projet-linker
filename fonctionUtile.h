#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

char* lire_nom(Elf32_Ehdr structElf32,int numSection,FILE* fichierElf);

FILE * ouvrirFichier(char * nomFichier);

char* AccesTableNomSection(Elf32_Ehdr elfHdr,FILE * fichierElf);

Elf32_Ehdr lireHeaderElf(FILE * fichierElf);

Elf32_Shdr * accesTableDesHeaders(Elf32_Ehdr structElf32, FILE * fichierElf);

Elf32_Shdr RechercheSectionByName(FILE * fichierElf, char * nomSection, Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32);

void afficheSection(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);

Elf32_Shdr *  rechercherTablesReimplentation(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf);

Elf32_Rel * tabSymboleRel(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);

Elf32_Rela * tabSymboleRela(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);

void afficherRelocation(int r_info,int r_offset);
