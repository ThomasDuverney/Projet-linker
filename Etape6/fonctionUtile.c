#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "fonctionUtile.h"
#include <string.h>

char* lire_nom(Elf32_Ehdr structElf32,int numSection,FILE* fichierElf,char* TableNomSection){

     Elf32_Shdr structSectionHeader;

     char* name = "";

    fseek(fichierElf, structElf32.e_shoff + numSection * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&structSectionHeader, 1, sizeof(Elf32_Shdr), fichierElf);

    // print section name
    if (structSectionHeader.sh_name)
      name = TableNomSection + structSectionHeader.sh_name;

    return name;

}

FILE * ouvrirFichier(char * nomFichier){

    FILE* fichierElf = NULL;

    fichierElf = fopen (nomFichier, "rb" );

    if (fichierElf==NULL)
    {
        printf ("\n Echec ouverture du fichier\n");
        exit (1);
    }

    return fichierElf;

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
  if(tabNomSection == NULL){
    printf("Erreur alocation AccesTableNomSection ");
    return NULL;
  }

  fseek(fichierElf, sectHdr.sh_offset, SEEK_SET); //REVIENT DEBUT SECTION
  fread(tabNomSection, 1, sectHdr.sh_size, fichierElf);

  return tabNomSection;

}

Elf32_Ehdr lireHeaderElf(FILE * fichierElf){

    Elf32_Ehdr structElf32;

    if (!fread(&structElf32, sizeof(structElf32), 1, fichierElf))
    {
            printf("failed to read elf header\n");
            exit(1);
    }

    return structElf32;
}


Elf32_Shdr * accesTableDesHeaders(Elf32_Ehdr structElf32, FILE * fichierElf){
  Elf32_Shdr structSectionHeader;
  int i;
	Elf32_Shdr * tabHeaders = malloc(sizeof(Elf32_Shdr)*structElf32.e_shoff);
  if(tabHeaders == NULL)
    return NULL;

	fseek(fichierElf,structElf32.e_shoff,0);

   	//structElf32.e_shentsize <=> taille de la table des sections

  	for(i=0;i<structElf32.e_shnum;i++){

  		if (!fread(&structSectionHeader, sizeof(Elf32_Shdr), 1, fichierElf))
  		{
    		printf("failed to read elf section header\n");
    		exit(1);
  		}
  		tabHeaders[i]=structSectionHeader;

  	}

	return tabHeaders;
}

Elf32_Shdr RechercheSectionByName(FILE * fichierElf, char * nomSection, Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,char* TableNomSection){

	int j = 0;
	char * tempNom =lire_nom(structElf32,j,fichierElf,TableNomSection);

	while(j<structElf32.e_shnum && strcmp(nomSection,tempNom)){
		j++;
		tempNom =lire_nom(structElf32,j,fichierElf,TableNomSection);
	}

	if( j == structElf32.e_shnum){
		printf("Le nom de section recherché n'est pas disponible dans ce fichier");
		exit (1);

	}else{

		return tabHeaders[j];
	}

}

SectionInfos * RechercheSectionByType(int typeSection,int * size,ContenuElf * contenuElf){

  int j = 0;
  SectionInfos * tabSectionType;
  *size=0;

  while(j<contenuElf->hdrElf.e_shnum){
    
    if(typeSection == contenuElf->tabSections[j].tabHdrSections.sh_type){
        if(*size == 0){

            tabSectionType=malloc(sizeof(SectionInfos));
            if( tabSectionType == NULL ){
              return NULL;
            }
            else{
              tabSectionType[*size] = contenuElf->tabSections[j];
              (*size)++;
            }
        }else{
            SectionInfos * tabTemp;
            tabTemp=realloc(tabSectionType,sizeof(SectionInfos)*(*size+1));

            if ( tabTemp == NULL){
              return NULL;
            }else{
              tabSectionType=tabTemp;
              tabSectionType[*size] = contenuElf->tabSections[j];
              (*size)++;
            }
        }
    }
    j++;
  }

  if( *size == 0){
    printf("Le type de section recherché n'est pas disponible dans ce fichier");
    exit (1);

  }else{
    return tabSectionType;
  }
}

void afficheSection(unsigned char * contenuSection,Elf32_Word  taille,FILE * fichierElf){

  if(contenuSection!= NULL){
  	int i;
  	for(i=0;i<taille;i++){

  		if(i%4 == 0 && i!=0){
  		  printf("     ");
  		}
  		if(i%16 == 0 && i!=0){
  			printf("\n");
  		}
  		printf("%02x",contenuSection[i]);
  	}
  }else{
    printf("Cette section n'a aucune donnée");
  }
}

unsigned char * RemplirContenuSection(Elf32_Shdr hdrSection,FILE * fichierElf){

  if(hdrSection.sh_size>0){
    unsigned char * contenuSection = malloc(hdrSection.sh_size);
    if(contenuSection == NULL){
        return NULL;
    }
    fseek(fichierElf,hdrSection.sh_offset, SEEK_SET);
    fread(contenuSection, 1, hdrSection.sh_size, fichierElf);

    return contenuSection;

  }else{
    return NULL;
  }
}
void afficherRelocation(int r_info,int r_offset){
	printf("offset\t");
	printf("\tInfo\t");
	printf("\tType\t");
	printf("\tVal.-Sym\n");

	printf("%08x\t",r_offset);
	printf("%08x\t",r_info);


	switch(ELF32_R_TYPE(r_info)){

		case 0:
			printf("R_ARM_NONE");
			break;
		case 1:
			printf("R_ARM_PC24");
			break;
		case 2:
			printf("R_ARM_ABS32");
			break;
		case 3:
			printf("R_ARM_REL32");
			break;
		case 28:
			printf("R_ARM_CALL");
			break;
		case 29:
			printf("R_ARM_JUMP24");
			break;
		default:
			printf("Ne correspont à aucun type");
			break;
	}
	printf("\t");

	printf("%08x\t",ELF32_R_SYM(r_info));

	printf("\n");

}
Elf32_Shdr *  rechercherTablesReimplentation(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf){

	Elf32_Shdr * tabSectionReimp;
	*size=0;
	int j;

	for(j=0;j<structElf32.e_shnum; j++){

		if(tabHeaders[j].sh_type == SHT_RELA || tabHeaders[j].sh_type == SHT_REL){
			if(*size == 0){

				tabSectionReimp=malloc(sizeof(Elf32_Shdr));
				if( tabSectionReimp == NULL ){
					return NULL;
				}
				else{
					tabSectionReimp[*size] = tabHeaders[j];
					(*size)++;
				}
			}else{
				Elf32_Shdr * tabTemp;
				tabTemp=realloc(tabSectionReimp,sizeof(Elf32_Shdr)*(*size+1));

				if ( tabTemp == NULL){
					return NULL;
				}else{
					tabSectionReimp=tabTemp;
					tabSectionReimp[*size] = tabHeaders[j];
					(*size)++;
				}
			}


		}

	}

	return tabSectionReimp;

}



Elf32_Rel * tabSymboleRel(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf){

	Elf32_Rel * tabSymbolRel = malloc(taille);
	Elf32_Rel  tempSymb;

	if(tabSymbolRel == NULL){
		printf("Erreur allocation dans le tableau des symboles de réalocation");
		return NULL;
	}

	int i;
	fseek(fichierElf,position, SEEK_SET);
	for(i=0; i< taille/sizeof(Elf32_Rel);i++){

		fread(&tempSymb,1,sizeof(Elf32_Rel),fichierElf);
		tabSymbolRel[i]=tempSymb;

	}

	return tabSymbolRel;


}

Elf32_Rela * tabSymboleRela(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf){

	Elf32_Rela * tabSymbolRela = malloc(taille);
	Elf32_Rela  tempSymb;

	if(tabSymbolRela == NULL){
		printf("Erreur allocation dans le tableau des symboles de réalocation");
		return NULL;
	}

	int i;
	fseek(fichierElf,position, SEEK_SET);
	for(i=0; i< taille/sizeof(Elf32_Rela);i++){

		fread(&tempSymb,1,sizeof(Elf32_Rel),fichierElf);
		tabSymbolRela[i]=tempSymb;
	}

	return tabSymbolRela;

}

char * AccesTableString(Elf32_Shdr * tabHeaders,Elf32_Ehdr structElf32,int * size,FILE * fichierElf,char * TableNomSection){

	Elf32_Shdr HdrTableString;
	HdrTableString = RechercheSectionByName(fichierElf,".strtab",tabHeaders,structElf32,TableNomSection);

	Elf32_Off position = HdrTableString.sh_offset;
	Elf32_Word  taille = HdrTableString.sh_size;
	*size=taille;

	char * tabString = malloc(taille);

	if( tabString == NULL ){
    printf("Erreur d'allocation AccesTableString");
		return NULL;
	}

	fseek(fichierElf,position, SEEK_SET);
  	fread(tabString,1,taille,fichierElf);

  	return tabString;

}

char * LireNomSymb(char * tabString, int indexSymb){

	char * nomSymb;
	nomSymb = tabString + indexSymb;

	return nomSymb;
}

void remplirStructure(FILE * fichier,ContenuElf * contenuElf){

  int i;
  int sizeTabString,sizeTabReloc;

  contenuElf->fichierElf = fichier;
  contenuElf->hdrElf = lireHeaderElf(fichier);
  
  // CHARGER TABLE NOM SECTION DANS LA STRUCTURE
  char * TableNomSection = AccesTableNomSection(contenuElf->hdrElf, fichier);
  //
  
  contenuElf->tabSections = malloc(contenuElf->hdrElf.e_shnum*sizeof(SectionInfos));
  if( contenuElf->tabSections == NULL){
  	printf("Erreur Allocation");
  	exit(1);
  }
  	
  Elf32_Shdr *  tabHeaders = accesTableDesHeaders(contenuElf->hdrElf, fichier);

  for(i=0;i<contenuElf->hdrElf.e_shnum;i++){
      SectionInfos sectionInfos;
      sectionInfos.tabHdrSections = tabHeaders[i];
      sectionInfos.nomSection = lire_nom(contenuElf->hdrElf,i,fichier,TableNomSection);
      sectionInfos.contenuSection= RemplirContenuSection(sectionInfos.tabHdrSections,fichier);
      contenuElf->tabSections[i] = sectionInfos;
  }
  
  contenuElf->tableString =AccesTableString(tabHeaders,contenuElf->hdrElf,&sizeTabString,fichier,TableNomSection);
  contenuElf->sizeTabString = sizeTabString;
  contenuElf->tabRelocation=rechercherTablesReimplentation(tabHeaders,contenuElf->hdrElf, &sizeTabReloc,fichier);
  
  
}

