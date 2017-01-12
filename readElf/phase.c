#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include <ctype.h>
#include "fonctionUtile.h"
#include "phase.h"


void fonctionEtape1(Elf32_Ehdr structElf32){

    if(structElf32.e_ident[EI_MAG0] ==0x7f && structElf32.e_ident[EI_MAG1] =='E' && structElf32.e_ident[EI_MAG2] =='L' && structElf32.e_ident[EI_MAG3] =='F'){

	    printf("En-tête ELF : \n");

	// ------------LE NOMBRE MAGIQUE.
	    printf("Magique :\t");

	    for(int i=0;i<EI_NIDENT;++i)
		printf("%02x ",structElf32.e_ident[i]);

	// ------------LA CLASSE
		printf("\nClass :\t");
		if(structElf32.e_ident[EI_CLASS]==1)
			printf("32-bit\n");
		else if(structElf32.e_ident[EI_CLASS] == 2)
	       	  	printf("64-bit\n");
	       	else
			printf("Invalid class\n");

	// ------------LES DONNEES
		printf("Données :\t");
		if(structElf32.e_ident[EI_DATA]==1)
			printf("little Indian\n");
		else if(structElf32.e_ident[EI_DATA] == 2)
	       	printf("Big Indian\n");
	    else
			printf("Invalid data encoding\n");

	// ------------LA VERSION
		printf("Version :\t %i ",structElf32.e_ident[EI_VERSION]);
		if(structElf32.e_version == 0)
			  printf("(Invalid version)\n");
	       	else
	       		printf("(Current version)\n");

	// ------------OS/ABI
		printf("OS/ABI :\t");
		if(structElf32.e_ident[EI_OSABI] == 0)
			printf("System V\n");
		else if(structElf32.e_ident[EI_OSABI] == 1)
	       	printf("HP-UX\n");
	    else if(structElf32.e_ident[EI_OSABI] == 2)
	       	printf("NETbsd\n");
	    else if(structElf32.e_ident[EI_OSABI] == 3)
	       	printf("LINUX\n");
	    else
	       	printf("autres ou inconnu\n");

	// ------------LA MACHINE
	    printf("Machine :\t");

	    if(structElf32.e_machine == 1)
		  printf("AT&T WE 32100\n");
	    else if(structElf32.e_machine == 2)
	       	  printf("SPARC\n");
	    else if(structElf32.e_machine == 3)
	    	  printf("Intel 80386\n");
	    else if(structElf32.e_machine == 4)
	    	  printf("Motorola 68000\n");
	    else if(structElf32.e_machine == 5)
	    	  printf("Motorola 88000\n");
	    else if(structElf32.e_machine == 6)
	    	  printf("Intel 80860\n");
	    else if(structElf32.e_machine == 40)
	    	  printf("ARM\n");
	    else
			  printf("Inconnu ou non spécifie\n");

	// -----------LE TYPE
	    printf("Type :\t");
	    if(structElf32.e_type == 1)
			printf("Relocatable (REL)\n");
	    else if(structElf32.e_type == 2)
			printf("Executable (EXEC)\n");
	    else if(structElf32.e_type == 3)
			printf("Shared Object file (DYN)\n");
			else if(structElf32.e_type == 4)
			printf("Core file (CORE) \n");
	    else if(structElf32.e_type == 0)
			printf("No file type (NONE)\n");
	    else
			printf("Inconnu ou non spécifie\n");


	// -----------LA TAILLE DU HEADER
	    printf("Taille :\t %i (Bytes) \n",structElf32.e_ehsize);

	// -----------DEBUT DE LA SECTION
	    printf("Début des en-têtes de section :\t %i (octet dans le fichier) \n",structElf32.e_shoff);

	// -----------LA TABLE DES CHAINES
	    printf("Table d'indexes des chaînes d'en-tête de section :\t %i\n",structElf32.e_shstrndx);

    }
	printf("\n\n");
}
void fonctionEtape2(Elf32_Ehdr structElf32, FILE * fichierElf,char* TableNomSection,Elf32_Shdr * TabHeaders){

	int i;
	Elf32_Shdr structSectionHeader;
   	fseek(fichierElf,structElf32.e_shoff,0);

  	printf("N ||%-18s||Type    ||Addr    ||Décal ||Taille||ES||Fl||L ||I ||A\n","Nom");

  	for(i=0;i<structElf32.e_shnum;i++){

		structSectionHeader = TabHeaders[i];
		//Numéro
  		printf("%-2i||",i);

  		//nom
  		printf("%-18s||",lire_nom(structElf32,i,fichierElf,TableNomSection));

  		//Type
  		switch(structSectionHeader.sh_type){

  			case SHT_NULL:
  				printf("NULL    ");
  				break;
  			case SHT_PROGBITS:
  				printf("PROGBITS");
  				break;
  			case SHT_SYMTAB:
  				printf("SYMTAB  ");
  				break;
  			case SHT_STRTAB:
  				printf("STRTAB  ");
  				break;
  			case SHT_RELA:
  				printf("RELA    ");
  				break;
  			case SHT_HASH:
  				printf("HASH    ");
  				break;
  			case SHT_DYNAMIC:
  				printf("DYNAMIC ");
  				break;
  			case SHT_NOTE:
  				printf("NOTE    ");
  				break;
  			case SHT_NOBITS:
  				printf("NOBITS  ");
  				break;
  			case SHT_REL:
  				printf("REL     ");
  				break;
  			case SHT_SHLIB:
  				printf("SHLIB   ");
  				break;
  			case SHT_DYNSYM:
  				printf("DYNSYM  ");
  				break;
  			case SHT_LOPROC:
  				printf("LOPROC  ");
  				break;
  			case SHT_HIPROC:
  				printf("HIPROC  ");
  				break;
  			case SHT_NUM:
  				printf("NUM     ");
  				break;
  			case SHT_LOUSER:
  				printf("LOUSER  ");
  				break;
  			case SHT_HIUSER:
  				printf("HIUSER  ");
  			default:
  				printf("Others  ");
  				break;

  		}
  		printf("||");
  		//adresse
  		printf("%8.8x||",structSectionHeader.sh_addr);
  		//décalage
  		printf("%6.6x||",structSectionHeader.sh_offset);
  		//taille
  		printf("%6.6x||",structSectionHeader.sh_size);
  		//ES
  		printf("%2.2x||",structSectionHeader.sh_entsize);
  		//Flags
  		char* r = malloc(sizeof(char)*12);
  		if (structSectionHeader.sh_flags & 0x1)
  			r = strcat(r,"W");
  		if (structSectionHeader.sh_flags & 0x2)
  			r = strcat(r,"A");
  		if (structSectionHeader.sh_flags & 0x4)
  			r = strcat(r,"X");
  		if (structSectionHeader.sh_flags & 0x10)
  			r = strcat(r,"M");
  		if (structSectionHeader.sh_flags & 0x20)
  			r = strcat(r,"S");
  		if (structSectionHeader.sh_flags & 0x40)
  			r = strcat(r,"I");
  		if (structSectionHeader.sh_flags & 0x80)
  			r = strcat(r,"L");
  		if (structSectionHeader.sh_flags & 0x100)
  			r = strcat(r,"O");
  		if (structSectionHeader.sh_flags & 0x200)
  			r = strcat(r,"G");
  		if (structSectionHeader.sh_flags & 0x400)
  			r = strcat(r,"T");
  		if (structSectionHeader.sh_flags & 0x0ff00000)
  			r = strcat(r,"O");
  		if (structSectionHeader.sh_flags & 0xf0000000)
  			r = strcat(r,"P");
  		if(strlen(r)==0)
  			r = strcat(r,"  ");
  		printf("%-2s||",r);
      free(r);
  		//Link
  		printf("%-2i||",structSectionHeader.sh_link);
  		//Info
  		printf("%-2i||",structSectionHeader.sh_info);
  		//Allignement
  		printf("%-2i",structSectionHeader.sh_addralign);


  	printf("\n");

  	}
   	printf("\nN = Numéro, Fl = Flags, L = Link, I = Info, A = Allignement\n");

	printf("\n\n");

}

void fonctionEtape3(FILE * fichierElf,char * section,Elf32_Ehdr structElf32,char* TableNomSection,Elf32_Shdr * tabHeaders){

	Elf32_Shdr tempHed;
	printf("Affichage de la section : %s \n",section);

	if(isdigit(*section) == 0){

		tempHed = RechercheSectionByName(fichierElf,section,tabHeaders,structElf32,TableNomSection);
		unsigned char * contenuSection = RemplirContenuSection(tempHed,fichierElf);
		afficheSection(contenuSection,tempHed.sh_size,fichierElf);
		free(contenuSection);

	}else{
		tempHed = tabHeaders[atoi(section)];
		unsigned char * contenuSection = RemplirContenuSection(tempHed,fichierElf);
		afficheSection(contenuSection,tempHed.sh_size,fichierElf);
		free(contenuSection);
	}

	printf("\n");
}

void fonctionEtape4(Elf32_Sym * tabSymb,int symTableSize,char * tabString){

	Elf32_Sym symbole;

	printf("Table des symboles\nNuméro || Valeur ||Taille|| Type  ||Portée|| Nom\n");

	for(int i=0; i<symTableSize; i++){
		symbole = tabSymb[i];

		// Affichage du numéro de section
		printf("  %-2d   ||", i);

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

		printf("\n\n");
}

void fonctionEtape5(Elf32_Ehdr structElf32,FILE * fichierElf,Elf32_Shdr * tabHeaders,Elf32_Shdr * tabReal,int tabRealSize){

	int i;
	int j;
	Elf32_Rel * tabSymRel; // Tableau symbole relocation rel
	Elf32_Rela * tabSymRela;// Tableau symbole relocation rela

	if(tabRealSize!=0){

		// CREATION TABLEAU DES SYMBOLES POUR CHAQUE SECTION REALOCATION
		for(i =0 ; i < tabRealSize; i++){

			if(tabReal[i].sh_type == SHT_REL){

				tabSymRel=AccesTabSymboleRel(tabReal[i].sh_offset,tabReal[i].sh_size,fichierElf);

				printf("offset\t");
				printf("\tInfo\t");
				printf("\tType\t");
				printf("\tVal.-Sym\n");
				for(j =0 ; j < tabReal[i].sh_size/sizeof(Elf32_Rel) ; j++){
					afficherRelocation(tabSymRel[j].r_info,tabSymRel[j].r_offset);
				}
				free(tabSymRel);
			}
			else{
				tabSymRela=AccesTabSymboleRela(tabReal[i].sh_offset,tabReal[i].sh_size,fichierElf);


				for(j =0 ; j < tabReal[i].sh_size/sizeof(Elf32_Rel) ; j++){
					afficherRelocation(tabSymRela[j].r_info,tabSymRela[j].r_offset);
				}

				free(tabSymRela);
			}
		}
	}else{
			printf("Ce fichier ne contient aucun repositionnement\n");
			exit(1);
	}

		printf("\n\n");
}


void fonctionEtape6(ContenuFus * contenuFus,SectionInfos ** tabSectionProgb1,SectionInfos ** tabSectionProgb2,int * size1,int * size2){

	*tabSectionProgb1 = RechercheSectionByType(SHT_PROGBITS,size1,contenuFus->contenuElf1);
	*tabSectionProgb2 = RechercheSectionByType(SHT_PROGBITS,size2,contenuFus->contenuElf2);

	fusionSection(*tabSectionProgb1,*tabSectionProgb2,*size1,*size2,contenuFus);

	// Affichage des sections
	printf("\x1b[34mAffichage des sections du fichier 1 \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElf1);

	printf("\x1b[34mAffichage des sections du fichier 2 \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElf2);

	printf("\x1b[34mAffichage des sections du fichier fusionné \x1b[0m\n");
	afficherVerifFusion(contenuFus->contenuElfFinal);
}

void fonctionEtape7(ContenuFus * contenuFus,SectionInfos ** tabSectionStr1,SectionInfos ** tabSectionStr2,int * size1,int * size2){

  *tabSectionStr1 = RechercheSectionByType(SHT_STRTAB,size1,contenuFus->contenuElf1);
  *tabSectionStr2 = RechercheSectionByType(SHT_STRTAB,size2,contenuFus->contenuElf2);

  fusionSection(*tabSectionStr1,*tabSectionStr2,*size1,*size2,contenuFus);

  int i;
  int sizetabsec = contenuFus->contenuElfFinal->sizeSections;
  while(i<sizetabsec && strcmp(contenuFus->contenuElfFinal->tabSections[i].nomSection,".strtab")){
    i++;
  }
  if(i!=sizetabsec){
    contenuFus->contenuElfFinal->tableString = (char *) contenuFus->contenuElfFinal->tabSections[i].contenuSection;
  }

  contenuFus->contenuElfFinal->tabSymb = malloc(sizeof(Elf32_Sym));
  contenuFus->contenuElfFinal->symTableSize = 0;

  int firstGlobal_1 = fusionTabSymLocaux(contenuFus->contenuElf1,contenuFus->contenuElfFinal);
  int firstGlobal_2 = fusionTabSymLocaux(contenuFus->contenuElf2,contenuFus->contenuElfFinal);
  updateIndexSymb(firstGlobal_1,contenuFus);
  fusionTabSymGlobal(firstGlobal_1,firstGlobal_2,contenuFus);

  printf("\x1b[34m Affichage de la table des symboles fichier 1 \x1b[0m\n");
  fonctionEtape4(contenuFus->contenuElf1->tabSymb,contenuFus->contenuElf1->symTableSize,contenuFus->contenuElf1->tableString);

  printf("\x1b[34m Affichage de la table des symboles fichier 2 \x1b[0m\n");;
  fonctionEtape4(contenuFus->contenuElf2->tabSymb,contenuFus->contenuElf2->symTableSize,contenuFus->contenuElf2->tableString);

  printf("\x1b[34m Affichage de la table des symboles fusionée \x1b[0m\n");
  fonctionEtape4(contenuFus->contenuElfFinal->tabSymb,contenuFus->contenuElfFinal->symTableSize,contenuFus->contenuElfFinal->tableString);
}
