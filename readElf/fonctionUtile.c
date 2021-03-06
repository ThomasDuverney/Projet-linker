#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "fonctionUtile.h"
#include <string.h>

char* lire_nom(Elf32_Ehdr structElf32,int numSection,FILE* fichierElf,char* TableNomSection){

     Elf32_Shdr structSectionHeader;

     char* name = ""/*strdup("")*/;

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

  fseek(fichierElf, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof sectHdr, SEEK_SET);
  fread(&sectHdr, 1, sizeof (sectHdr), fichierElf);

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
  SectionInfos newSectionInfos;

  while(j<contenuElf->sizeSections){

    if(typeSection == contenuElf->tabSections[j].tabHdrSections.sh_type){
        if(*size == 0){

            tabSectionType=malloc(sizeof(SectionInfos));
            if( tabSectionType != NULL ){

	             dupliquerSectionInfos(&newSectionInfos,&(contenuElf->tabSections[j]));
               tabSectionType[*size] = newSectionInfos;
               (*size)++;

            }
        }else{
            SectionInfos * tabTemp;
            tabTemp=realloc(tabSectionType,sizeof(SectionInfos)*(*size+1));

            if ( tabTemp != NULL){
              tabSectionType=tabTemp;
              dupliquerSectionInfos(&newSectionInfos,&(contenuElf->tabSections[j]));
              tabSectionType[*size] = newSectionInfos;
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
    printf("Cette section n'a aucune donnée\n");
  }
}
Elf32_Sym * AccesTableSymbole(FILE * fichierElf,Elf32_Ehdr structElf32,int * symTableSize,char* TableNomSection,Elf32_Shdr * tabHeaders){

	Elf32_Shdr structSymTable;
	structSymTable = RechercheSectionByName(fichierElf,".symtab",tabHeaders,structElf32, TableNomSection);

	*symTableSize = structSymTable.sh_size/structSymTable.sh_entsize;
	Elf32_Sym symbole;
	Elf32_Sym * tabSymb = malloc(structSymTable.sh_size);
	if(tabSymb != NULL){

		fseek(fichierElf,structSymTable.sh_offset,0);

		for(int i=0; i<*symTableSize; i++){
			fread(&symbole, sizeof(symbole), 1, fichierElf);
			tabSymb[i] = symbole;
		}
	}
	return tabSymb;
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



Elf32_Rel * AccesTabSymboleRel(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf){

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

Elf32_Rela * AccesTabSymboleRela(Elf32_Off position,Elf32_Word  taille,FILE * fichierElf){

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

void remplirStructure(FILE * fichier,ContenuElf * contenuElf,Elf32_Shdr ** TabHeaders){

  int i;

  contenuElf->fichierElf = fichier;

  contenuElf->hdrElf = lireHeaderElf(fichier);

  *TabHeaders = accesTableDesHeaders(contenuElf->hdrElf, fichier);

  // CHARGER TABLE NOM SECTION DANS LA STRUCTURE
  contenuElf->TableNomSection = AccesTableNomSection(contenuElf->hdrElf, fichier);


  contenuElf->tableString = AccesTableString(*TabHeaders, contenuElf->hdrElf ,&(contenuElf->sizeTabString),fichier,contenuElf->TableNomSection);


  contenuElf->tabSymb = AccesTableSymbole(fichier, contenuElf->hdrElf ,&(contenuElf->symTableSize),contenuElf->TableNomSection,*TabHeaders);


  contenuElf->tabRela = rechercherTablesReimplentation(*TabHeaders, contenuElf->hdrElf ,&(contenuElf->tabRelaSize),fichier);

  contenuElf->tabSections = malloc(contenuElf->hdrElf.e_shnum*sizeof(SectionInfos));
  if( contenuElf->tabSections == NULL){
  	printf("Erreur Allocation");
  	exit(1);
  }
  contenuElf->sizeSections = contenuElf->hdrElf.e_shnum;

  for(i=0;i<contenuElf->sizeSections;i++){
      SectionInfos sectionInfos;
      sectionInfos.tabHdrSections = (*TabHeaders)[i];
      sectionInfos.nomSection = lire_nom(contenuElf->hdrElf,i,fichier,contenuElf->TableNomSection);
      sectionInfos.contenuSection= RemplirContenuSection(sectionInfos.tabHdrSections,fichier);
      contenuElf->tabSections[i] = sectionInfos;
  }


}

void CopieSectionInfos(ContenuFus * contenuFus,const SectionInfos * sectionInfos){

				SectionInfos newSectionInfos;
				if(contenuFus->contenuElfFinal->sizeSections == 0){
					contenuFus->contenuElfFinal->tabSections = malloc(sizeof(SectionInfos));
					if( contenuFus->contenuElfFinal->tabSections  != NULL){

						dupliquerSectionInfos(&newSectionInfos,sectionInfos);
						contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections] = newSectionInfos;
        				        contenuFus->contenuElfFinal->sizeSections++;
					}

				}else{
					SectionInfos * tabTemp;
					tabTemp=realloc(contenuFus->contenuElfFinal->tabSections,sizeof(SectionInfos)*(contenuFus->contenuElfFinal->sizeSections+1));

					if ( tabTemp != NULL){

						contenuFus->contenuElfFinal->tabSections=tabTemp;
						dupliquerSectionInfos(&newSectionInfos,sectionInfos);
						contenuFus->contenuElfFinal->tabSections[contenuFus->contenuElfFinal->sizeSections] = newSectionInfos;
						contenuFus->contenuElfFinal->sizeSections++;
					}
				}
}

void dupliquerSectionInfos(SectionInfos  * newSectionInfos,const SectionInfos * sectionInfos){

	newSectionInfos->nomSection = malloc(strlen(sectionInfos->nomSection)*sizeof(char));
	strcpy(newSectionInfos->nomSection,sectionInfos->nomSection);

	newSectionInfos->tabHdrSections = sectionInfos->tabHdrSections;

	newSectionInfos->contenuSection = malloc(sectionInfos->tabHdrSections.sh_size);
	memcpy(newSectionInfos->contenuSection,sectionInfos->contenuSection,sectionInfos->tabHdrSections.sh_size);

}

void fusionSection(SectionInfos * tabSection1,SectionInfos * tabSection2,int size1, int size2,ContenuFus * contenuFus){

	int i,k;
  int flag=0;
	int sizeWrited = 0;
	int newSectionSize = 0;
	int * tabWrited = malloc(size2+size1*sizeof(int));

	contenuFus->contenuElfFinal->sizeSections = 0;

	for(k=0;k<size1;k++){
    flag =0;
		for(i=0;i<size2;i++){


			if(strcmp(tabSection1[k].nomSection,tabSection2[i].nomSection) == 0){

				//concatène et ecrit dans le header de la section progb1 la nouvelle taille
				CopieSectionInfos(contenuFus,&(tabSection1[k]));

			 	newSectionSize = tabSection1[k].tabHdrSections.sh_size + tabSection2[i].tabHdrSections.sh_size;
			 	if(newSectionSize !=0){
			  		unsigned char * tabTemp = realloc(contenuFus->contenuElfFinal->tabSections[(contenuFus->contenuElfFinal->sizeSections)-1].contenuSection,newSectionSize);

					if(tabTemp!=NULL){

						int j;
						for(j=0;j<tabSection2[i].tabHdrSections.sh_size;j++){
								tabTemp[tabSection1[k].tabHdrSections.sh_size+j]=tabSection2[i].contenuSection[j];
						}
						contenuFus->contenuElfFinal->tabSections[(contenuFus->contenuElfFinal->sizeSections)-1].contenuSection = tabTemp;
						contenuFus->contenuElfFinal->tabSections[(contenuFus->contenuElfFinal->sizeSections)-1].tabHdrSections.sh_size = newSectionSize;

				 	 }
			  }

			  flag = 1;
			  tabWrited[sizeWrited]=i;
			  sizeWrited++;
			}
		}
		if(flag == 0){
			CopieSectionInfos(contenuFus,&(tabSection1[k]));
		}

	}
	for(k=0;k<size2;k++){
		i=0;
		while(i<sizeWrited && tabWrited[i] != k){
			i++;
		}
		if(sizeWrited == i){
			CopieSectionInfos(contenuFus,&(tabSection2[k]));
		}
	}
	free(tabWrited);
}

void afficherVerifFusion(ContenuElf* contenuElf){
		for(int i=0; i<contenuElf->sizeSections; i++){
			printf("Section %s :\n", contenuElf->tabSections[i].nomSection);
			afficheSection(contenuElf->tabSections[i].contenuSection, contenuElf->tabSections[i].tabHdrSections.sh_size, contenuElf->fichierElf);
			printf("\n\n");
		}
		printf("\n");
}

void afficherLesContenusSections(ContenuElf* contenuElf){
		for(int i=0; i<contenuElf->sizeSections; i++){
			printf("Section %s :\n", contenuElf->tabSections[i].nomSection);
			afficheSection(contenuElf->tabSections[i].contenuSection, contenuElf->tabSections[i].tabHdrSections.sh_size, contenuElf->fichierElf);
			printf("\n\n");
		}
		printf("\n");
}

/*
	Remplit une structure de type ContenuFus à l'aide de deux fichiers
	fichDest, FILE * : le premier fichier
	secondFich, FILE * : le second fichier
	retour ContenuFus * : la structure créée remplie
*/
ContenuFus* remplirStructureFusion(FILE * fichDest, FILE * secondFich){
			ContenuFus * contenuFus = malloc(sizeof(ContenuFus));

			contenuFus->contenuElf1 = malloc(sizeof(ContenuElf));
			contenuFus->contenuElf2 = malloc(sizeof(ContenuElf));
			contenuFus->contenuElfFinal = malloc(sizeof(ContenuElf));

			Elf32_Shdr * TabHeaders1 = NULL;
			Elf32_Shdr * TabHeaders2 = NULL;

			remplirStructure(fichDest,contenuFus->contenuElf1,&TabHeaders1);
			remplirStructure(secondFich,contenuFus->contenuElf2,&TabHeaders2);

			return contenuFus;
}

void libererSectionInfos(SectionInfos * sectionInfos,int sizeTabSection){
	int i;
	for (i=0;i<sizeTabSection;i++){
	   free(sectionInfos[i].contenuSection);
	}

	free(sectionInfos);
	printf("Mémoire libérée (sectionInfos)\n");
	}

void libererTabHeaders(Elf32_Shdr * TabHeaders){

	free(TabHeaders);
	printf("Mémoire libérée (TabHeaders)\n");

}

void libererContenuElf(ContenuElf * contenuElf){

	int i;

	for (i=0;i<contenuElf->sizeSections;i++){
		free((contenuElf->tabSections)[i].contenuSection);
	}

	free(contenuElf->tabSections);

  if(contenuElf->tableString != NULL)
    free(contenuElf->tableString);

  if(contenuElf->TableNomSection != NULL)
    free(contenuElf->TableNomSection);

  if(contenuElf->tabSymb != NULL)
    free(contenuElf->tabSymb);

  if(contenuElf->tabRela != NULL)
    free(contenuElf->tabRela);

  if(contenuElf->fichierElf != NULL)
  		fclose(contenuElf->fichierElf);

	free(contenuElf);

}


void libererMemoire(ContenuFus * contenuFus){

	if(contenuFus->contenuElfFinal != NULL){

	int i;

    for (i=0;i<contenuFus->contenuElfFinal->sizeSections;i++){
      free((contenuFus->contenuElfFinal->tabSections)[i].nomSection);
    }

		libererContenuElf(contenuFus->contenuElfFinal);

	}
  if(contenuFus->contenuElf1 != NULL){

    libererContenuElf(contenuFus->contenuElf1);

  }

  if(contenuFus->contenuElf2 != NULL){

    libererContenuElf(contenuFus->contenuElf2);

  }

	free(contenuFus);

	printf("Mémoire libérée (ContenuFus)\n");

}


int fusionTabSymLocaux(ContenuElf * contenuElf,ContenuElf * contenuFusion){
	Elf32_Sym * tabSymbFile = contenuElf->tabSymb;
	int sizeTabFile = contenuElf->symTableSize;
	int i =0;

	while( i<sizeTabFile && ELF32_ST_BIND(tabSymbFile[i].st_info) == STB_LOCAL){
		ajoutTabSymb(contenuFusion,tabSymbFile[i]);
		i++;
  }

  return i;
}

void updateIndexSymb(int lastLocauxAdd,ContenuFus * contenuFus){
			Elf32_Sym ** tabSymbFile = &(contenuFus->contenuElfFinal->tabSymb);
			int sizeTabFile = contenuFus->contenuElfFinal->symTableSize;
			int i= lastLocauxAdd;

			for(; i<sizeTabFile ;i++){
				(*tabSymbFile)[i].st_name = (*tabSymbFile)[i].st_name + contenuFus->contenuElf1->sizeTabString;
			}
}


void fusionTabSymGlobal(int firstGlobal_1,int firstGlobal_2,ContenuFus * contenuFus){
	Elf32_Sym * tabSymbFile_1 = contenuFus->contenuElf1->tabSymb;
	int sizeTabFile_1 = contenuFus->contenuElf1->symTableSize;

	Elf32_Sym * tabSymbFile_2 = contenuFus->contenuElf2->tabSymb;
	int sizeTabFile_2 = contenuFus->contenuElf2->symTableSize;

	int * sizeTableSym = &(contenuFus->contenuElfFinal->symTableSize);
	int i= firstGlobal_1;
	int j= firstGlobal_2;
	char * nomSymb_1;
	char * nomSymb_2;

	int sizeWrited = 0;
	int flag = 0;
	int nbMaxGlobaux = (sizeTabFile_1 -firstGlobal_1) + (sizeTabFile_2 -firstGlobal_2);
	int * tabWrited = malloc(sizeof(int)*nbMaxGlobaux);

	for(; i<sizeTabFile_1 ;i++){
			j= firstGlobal_2;
			flag = 0;
			for(;j < sizeTabFile_2;j++){

				nomSymb_1 = LireNomSymb((char *)contenuFus->contenuElf1->tableString, tabSymbFile_1[i].st_name);
				nomSymb_2 = LireNomSymb((char *)contenuFus->contenuElf2->tableString, tabSymbFile_2[j].st_name);

				// SI LES SYMBOLES SONT GLOBAUX ET SONT DE MEME NOM
				if(strcmp(nomSymb_1,nomSymb_2) == 0){

					if(tabSymbFile_1[i].st_shndx != SHN_UNDEF && tabSymbFile_2[j].st_shndx != SHN_UNDEF){
							ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_1[i]);

					}else if(tabSymbFile_1[i].st_shndx != SHN_UNDEF && tabSymbFile_2[j].st_shndx == SHN_UNDEF){
						  ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_1[i]);


					}else if(tabSymbFile_1[i].st_shndx == SHN_UNDEF && tabSymbFile_2[j].st_shndx != SHN_UNDEF){
							ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_2[j]);
							contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name = contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name
																																	+ contenuFus->contenuElf1->sizeTabString;

					}else{
						printf("Deux Symboles globaux défini avec le même nom sont présents dans la table des entrées\n");
						printf("Echec de l'édition des liens\n");
						exit(1);
					}
					flag =1;
					tabWrited[sizeWrited] = j;
					sizeWrited++;
			}
			if(flag == 0){
					ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_1[i]);
					flag =1;
			}
		}
	}

	j= firstGlobal_2;

	for(;j< sizeTabFile_2;j++){
		i=0;
		while(i<sizeWrited && tabWrited[i] != j){
			i++;

		}
		if(sizeWrited == i){
				ajoutTabSymb(contenuFus->contenuElfFinal,tabSymbFile_2[j]);
				contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name = contenuFus->contenuElfFinal->tabSymb[(*sizeTableSym)-1].st_name
																														+ contenuFus->contenuElf1->sizeTabString;
		}
	}
  printf("Taille tab %i",contenuFus->contenuElfFinal->symTableSize);
	free(tabWrited);
}

void ajoutTabSymb(ContenuElf * contenuFusion,Elf32_Sym symbAdd){

	int * sizeTableSym = &(contenuFusion->symTableSize);

	if((*sizeTableSym) > 0 ){
		Elf32_Sym * tabTemp = realloc(contenuFusion->tabSymb,sizeof(Elf32_Sym)*((*sizeTableSym)+1));
		if(tabTemp!=NULL){
				contenuFusion->tabSymb = tabTemp;
				contenuFusion->tabSymb[*sizeTableSym] = symbAdd;
				(*sizeTableSym)++;
		}
	}else{
		contenuFusion->tabSymb[*sizeTableSym] = symbAdd;
		(*sizeTableSym)++;
	}

}
