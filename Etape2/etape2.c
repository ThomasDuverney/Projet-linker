#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

//<=> readelf -S file1.o

char* lire_nom(Elf32_Ehdr structElf32, Elf32_Shdr structSectionHeader,int numSection,FILE* fichierElf);

char* AccesTableNomSection(Elf32_Ehdr elfHdr,FILE * fichierElf);

Elf32_Ehdr lireHeaderElf(char *argv[]);

int main(int argc, char *argv[]){
	
	FILE* fichierElf = NULL;
	//Elf32_Ehdr structElf32;
	Elf32_Shdr structSectionHeader;
	int i;//variable de boucle
    
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
   	
   	//structElf32.e_shentsize <=> taille de la table des sections
   	
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
  		printf("%s||",lire_nom(structElf32,structSectionHeader,i,fichierElf));
  		
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
	return 0;
	
}

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
