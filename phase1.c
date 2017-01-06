#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include <ctype.h>
#include "fonctionUtile.h"



void fonctionEtape1(Elf32_Ehdr structElf32){
   	
    if(structElf32.e_ident[EI_MAG0] ==0x7f && structElf32.e_ident[EI_MAG1] =='E' && structElf32.e_ident[EI_MAG2] =='L' && structElf32.e_ident[EI_MAG3] =='F'){
    
	    printf("En-tête ELF : \n");
	    
	// ------------LE NOMBRE MAGIQUE    
	    printf("Magique :\t");
     
	    for(int i=0;i<EI_NIDENT;++i) 
		printf("%x",structElf32.e_ident[i]);
		
	// ------------LA CLASSE
		printf("\nClass :\t");
		if(structElf32.e_ident[EI_CLASS]==1)
			printf("32-bit\n");
		else if(structElf32.e_ident[EI_CLASS] == 2)
	       	  	printf("64-bit\n");
	       	else
			printf("Invalid class\n");
			
	// ------------LES DONNEES
		printf("\nDonnées :\t");
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
	    else if(structElf32.e_machine == 4)-h
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
			printf("Relocatable\n");
	    else if(structElf32.e_type == 2)
			printf("Executable\n");
	    else if(structElf32.e_type == 3)
			printf("Shared Object\n");
	    else if(structElf32.e_type == 0)
			printf("No file type\n");
	    else
			printf("Inconnu ou non spécifie\n");
        
	     
	// -----------LA TAILLE DU HEADER
	    printf("Taille :\t %i (Bytes) \n",structElf32.e_ehsize);
	    
	// -----------DEBUT DE LA SECTION
	    printf("Début des en-têtes de section :\t %i (octet dans le fichier) \n",structElf32.e_shoff);
	    
	// -----------LA TABLE DES CHAINES 
	    printf("Table d'indexes des chaînes d'en-tête de section :\t %i\n",structElf32.e_shstrndx);   	  

    }

}
void fonctionEtape2(Elf32_Ehdr structElf32, FILE * fichierElf){

	//structElf32.e_shentsize <=> taille de la table des sections
	int i;
	Elf32_Shdr structSectionHeader;
   	fseek(fichierElf,structElf32.e_shoff,0); 
  	printf("N||Nom    ||Type    ||Addr    ||Décal ||Taille||ES||Fl||L||I||A\n");

  	for(i=0;i<structElf32.e_shnum;i++){
  	
  		if (!fread(&structSectionHeader, sizeof(structSectionHeader), 1, fichierElf))
  		{
    		printf("failed to read elf section header\n");
    		exit(1);
  		}
		
		//Numéro
  		printf("%i||",i);
  		
  		//nom
  		printf("%s||",lire_nom(structElf32,i,fichierElf));
  		
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
  		printf("%s||",r);
  		
  		//Link
  		printf("%i||",structSectionHeader.sh_link);
  		//Info
  		printf("%i||",structSectionHeader.sh_info);
  		//Allignement
  		printf("%i",structSectionHeader.sh_addralign);

  	
  	printf("\n");
  	
  	}
   	printf("\nN = Numéro, Fl = Flags, L = Link, I = Info, A = Allignement\n");

}


void fonctionEtape5(Elf32_Ehdr structElf32,FILE * fichierElf){

	int size;
	Elf32_Shdr * tabReal;
	int i;
	int j;
	Elf32_Rel * tabSymRel; // Tableau symbole relocation rel
	Elf32_Rela * tabSymRela;// Tableau symbole relocation rela
	
	Elf32_Shdr * tabHeaders = accesTableDesHeaders(structElf32,fichierElf);

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

void fonctionEtape3(FILE * fichierElf,char * section,Elf32_Ehdr structElf32){
	
	Elf32_Shdr * tabHeaders = accesTableDesHeaders(structElf32,fichierElf);
	Elf32_Shdr tempHed;
	printf("Affichage de la section : %s \n",section);
	
	if(isalpha(*section) != 0){
	
		tempHed = RechercheSectionByName(fichierElf,section,tabHeaders,structElf32);
		afficheSection(tempHed.sh_offset,tempHed.sh_size,fichierElf);
	
	}else{
		
		Elf32_Shdr tempHed = tabHeaders[atoi(section)];
		afficheSection(tempHed.sh_offset,tempHed.sh_size,fichierElf);
		
	}
	
	printf("\n");
	
}  
