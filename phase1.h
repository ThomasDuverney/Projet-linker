#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

void fonctionEtape5(Elf32_Ehdr structElf32,FILE * fichierElf);

void fonctionEtape1(Elf32_Ehdr structElf32);

void fonctionEtape2(Elf32_Ehdr structElf32, FILE * fichierElf);

void fonctionEtape3(FILE * fichierElf,char * section,Elf32_Ehdr structElf32);
