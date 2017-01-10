#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

void fonctionEtape1(Elf32_Ehdr structElf32);

void fonctionEtape2(Elf32_Ehdr structElf32, FILE * fichierElf,char* TableNomSection,Elf32_Shdr * TabHeaders);

void fonctionEtape3(FILE * fichierElf,char * section,Elf32_Ehdr structElf32,char* TableNomSection,Elf32_Shdr * tabHeaders);

void fonctionEtape4(FILE * fichierElf,Elf32_Ehdr structElf32,char* TableNomSection,Elf32_Shdr * tabHeaders,Elf32_Sym * tabSymb,int symTableSize,char * tabString);

void fonctionEtape5(Elf32_Ehdr structElf32,FILE * fichierElf,Elf32_Shdr * tabHeaders,Elf32_Shdr * tabReal,int tabRealSize);
