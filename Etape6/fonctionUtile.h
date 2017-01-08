#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

typedef struct {
        char * nomSection;
        Elf32_Shdr  tabHdrSections;
        char * contenuSection;
} SectionInfos;


typedef struct {
        FILE * fichierElf;

        Elf32_Ehdr hdrElf;

        SectionInfos * tabSections;
        int sizeSections;

        char * tableString;
        Elf32_Shdr * tabRelocation;

}ContenuElf;

typedef struct {
        ContenuElf * ContenuElf1;
        ContenuElf * ContenuElf2;
} contenuFus;

/*
Retourne le nom d'une section dont le numéro est passée en paramètre.
*/
char* lire_nom(Elf32_Ehdr structElf32,int numSection,FILE* fichierElf);

/*Retourne un fichier ouvert en readbit dont le nom de fichier est passé en paramètre*/
FILE * ouvrirFichier(char * nomFichier);

/*Retourne la table des noms des sections*/
char* AccesTableNomSection(Elf32_Ehdr elfHdr,FILE * fichierElf);

/*Retourne le contenu d'une section*/
void remplirStructure(FILE * fichier,ContenuElf * contenuElf);

Elf32_Ehdr lireHeaderElf(FILE * fichierElf);
/*
Retourne un tableau de tout les headers de chaque sections.
*/
Elf32_Shdr * accesTableDesHeaders(Elf32_Ehdr structElf32, FILE * fichierElf);
/*
Retourne le headers d'une section dont le nom est passée en paramètre.
*/
Elf32_Shdr RechercheSectionByName(FILE * fichierElf, char * nomSection, Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32);
/*
Affiche le contenu d'une section dont la position et la taille est passé en paramètre.
*/
void afficheSection(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);
/*
Retourne un tableau de headers des sections qui sont de type Rel ou Rela, la taille du tableau retourner est passé par référence.
*/
Elf32_Shdr *  rechercherTablesReimplentation(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf);
/*
Renvoi un tableau des symbole d'une section de relocation de type Rel. la taille du tableau peut être calculé de cette manière: taille/sizeof(Elf32_Rel)
*/
Elf32_Rel * tabSymboleRel(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);
/*
Renvoi un tableau des symboles d'une section de relocation de type Rela. la taille du tableau peut être calculé de cette manière: taille/sizeof(Elf32_Rela)
*/
Elf32_Rela * tabSymboleRela(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf);
/*
Affiche les information de rélocation d'un symbole
*/
void afficherRelocation(int r_info,int r_offset);
/*
Retourne le nom d'un symbole dont l'index dans la table des string est passé en paramètre.
*/
char * LireNomSymb(char * tabString, int indexSymb);
/*
Retourne la table des string.
*/
char * AccesTableString(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf);
/*
Retourne un tableau de tout les headers de section du type donné en paramètre. Le type donné est le numéro de constant par exemple
pour le type SHT_PROGBITS = 1,  taille du tableau retourner est passé par référence.
*/
Elf32_Shdr * RechercheSectionByType(FILE * fichierElf,int typeSection,int * size, Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32);

void remplirStructure(FILE * fichier,ContenuElf * contenuElf);
